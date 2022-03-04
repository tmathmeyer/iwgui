#include "panels.h"

namespace iwgui {

WifiMenuPanel::WifiMenuPanel(std::shared_ptr<base::dbus::Connection> connection)
    : dbus_connection_(std::move(connection)) {
  station_ = dbus_connection_->GetInterface<iwd::Station>(
      "net.connman.iwd", "/net/connman/iwd/0/4");
  Reset();
}

WifiMenuPanel::WifiMenuSection* WifiMenuPanel::GetSection(size_t index) {
  while (index + 1 > sections_.size()) {
    WifiMenuPanel::WifiMenuSection* new_entry;
    auto new_frame =
        xpp::ui::FrameFactory<WifiMenuPanel::WifiMenuSection>::CaptureFrame(
            &new_entry, this);
    sections_.push_back(
        std::make_tuple<WifiMenuPanel::WifiMenuSection*, xpp::ui::FramePtr>(
            std::move(new_entry), std::move(new_frame)));
  }
  return std::get<0>(sections_[index]);
}

void WifiMenuPanel::OnPaint(xpp::ui::WindowProxy win, xpp::gfx::Graphics g) {
  g.SetColor(xpp::gfx::SDRColor::GRAY8);
  g.FillRect(0, 0, g.size().width, g.size().height);
  ssize_t margin = 12;
  ssize_t top = margin / 2;
  bool drawn_first = false;
  for (const auto& section : sections_) {
    if (drawn_first) {
      g.SetColor(xpp::gfx::SDRColor::GRAY3);
      g.FillRect(margin, top, g.size().width - margin * 2, 1);
    }
    drawn_first = true;
    if (g.size().width < (margin * 2))
      return;
    top += margin / 2 + 1;
    auto subg = g.SubArea({margin, top}, {g.size().width - (margin * 2),
                                          std::get<0>(section)->Height()});
    std::get<1>(section)->Draw(win, std::move(subg));
    top += std::get<0>(section)->Height();
    top += margin / 2;
  }
}

void WifiMenuPanel::OnKey(xpp::ui::WindowProxy win,
                          xpp::ui::KeyEvent ev,
                          int code) {}

void WifiMenuPanel::OnMotion(xpp::ui::WindowProxy win, xpp::gfx::Coord at) {
  ssize_t bottom = 0;
  const auto& section = GetSectionAt(at, &bottom);
  if (bottom != -1)
    std::get<1>(section)->NotifyMotion(win, {at.x, at.y - bottom});
}

void WifiMenuPanel::OnClick(xpp::ui::WindowProxy win,
                            xpp::ui::MouseButton button,
                            xpp::ui::ButtonAction action,
                            xpp::gfx::Coord at) {
  ssize_t bottom = 0;
  const auto& section = GetSectionAt(at, &bottom);
  if (bottom != -1)
    std::get<1>(section)->NotifyClick(win, button, action,
                                      {at.x, at.y - bottom});
}

const std::tuple<WifiMenuPanel::WifiMenuSection*, xpp::ui::FramePtr>&
WifiMenuPanel::GetSectionAt(xpp::gfx::Coord at, ssize_t* bottom) {
  static auto empty =
      std::make_tuple<WifiMenuPanel::WifiMenuSection*, xpp::ui::FramePtr>(
          nullptr, nullptr);
  ssize_t margin = 12;
  *bottom = margin / 2;
  ssize_t top = *bottom;
  for (const auto& section : sections_) {
    top += std::get<0>(section)->Height();
    if (at.y > *bottom && at.y < top)
      return section;
    *bottom = top + margin;
  }
  *bottom = -1;
  return empty;
}

void WifiMenuPanel::Reset() {
  auto* section = GetSection(1);
  section->Erase();
  station_->Scan();
  for (auto& net : station_->GetOrderedNetworks()) {
    section->CreateNextEntry(std::move(net));
  }
}

WifiMenuPanel::WifiMenuEntry::WifiMenuEntry(
    std::unique_ptr<iwd::Network> network,
    WifiMenuPanel* menu_weak_ref)
    : displayed_network_(std::move(network)),
      menu_panel_weak_ref_(menu_weak_ref) {}

void WifiMenuPanel::WifiMenuEntry::OnPaint(xpp::ui::WindowProxy win,
                                           xpp::gfx::Graphics g) {
  if (hovered_) {
    g.SetColor(xpp::gfx::SDRColor::BLUE);
  } else {
    g.SetColor(xpp::gfx::SDRColor::BLACK);
  }
  g.SetFont("Fantasque Sans Mono", 8);
  int text_height = g.RealFontHeight();
  g.DrawText(20,
             (text_height + WifiMenuPanel::WifiMenuEntry::kLineHeight) / 2 - 8,
             displayed_network_->Name());
  if (displayed_network_->Connected()) {
    g.FillRect(7, 27, 6, 6);
  }
}

void WifiMenuPanel::WifiMenuEntry::OnKey(xpp::ui::WindowProxy win,
                                         xpp::ui::KeyEvent ev,
                                         int code) {}

void WifiMenuPanel::WifiMenuEntry::OnMotion(xpp::ui::WindowProxy win,
                                            xpp::gfx::Coord at) {}

void WifiMenuPanel::WifiMenuEntry::OnClick(xpp::ui::WindowProxy win,
                                           xpp::ui::MouseButton button,
                                           xpp::ui::ButtonAction action,
                                           xpp::gfx::Coord coord) {
  if (action == xpp::ui::ButtonAction::kPress) {
    pressed_ = true;
  } else if (pressed_ && !displayed_network_->Connected()) {
    pressed_ = false;
    if (displayed_network_->GetKnownNetwork().has_value()) {
      displayed_network_->Connect();
      menu_panel_weak_ref_->Reset();
      win.Repaint();
    } else {
      std::cout << displayed_network_->Name() << " is not a known network\n";
    }
  }
}

void WifiMenuPanel::WifiMenuEntry::NotifyHoverMouseEnter() {
  hovered_ = true;
  pressed_ = false;
}

void WifiMenuPanel::WifiMenuEntry::NotifyHoverMouseExit() {
  hovered_ = false;
  pressed_ = false;
}

WifiMenuPanel::WifiMenuSection::WifiMenuSection(WifiMenuPanel* menu_weak_ref)
    : menu_panel_weak_ref_(menu_weak_ref) {}

void WifiMenuPanel::WifiMenuSection::Erase() {
  entries_.clear();
}

uint32_t WifiMenuPanel::WifiMenuSection::Height() {
  return WifiMenuPanel::WifiMenuEntry::kLineHeight * entries_.size();
}

WifiMenuPanel::WifiMenuEntry* WifiMenuPanel::WifiMenuSection::CreateNextEntry(
    std::unique_ptr<iwd::Network> network) {
  WifiMenuPanel::WifiMenuEntry* new_entry;
  auto new_frame =
      xpp::ui::FrameFactory<WifiMenuPanel::WifiMenuEntry>::CaptureFrame(
          &new_entry, std::move(network), menu_panel_weak_ref_);
  entries_.push_back(
      std::make_tuple<WifiMenuPanel::WifiMenuEntry*, xpp::ui::FramePtr>(
          std::move(new_entry), std::move(new_frame)));
  return std::get<0>(entries_[entries_.size() - 1]);
}

void WifiMenuPanel::WifiMenuSection::OnPaint(xpp::ui::WindowProxy win,
                                             xpp::gfx::Graphics g) {
  ssize_t top = 0;
  for (const auto& entry : entries_) {
    auto subg = g.SubArea(
        {0, top}, {g.size().width, WifiMenuPanel::WifiMenuEntry::kLineHeight});

    std::get<1>(entry)->Draw(win, std::move(subg));
    top += WifiMenuPanel::WifiMenuEntry::kLineHeight;
  }
}

void WifiMenuPanel::WifiMenuSection::OnKey(xpp::ui::WindowProxy win,
                                           xpp::ui::KeyEvent ev,
                                           int code) {}

void WifiMenuPanel::WifiMenuSection::OnMotion(xpp::ui::WindowProxy win,
                                              xpp::gfx::Coord at) {
  ssize_t bottom = 0;
  ssize_t top = bottom;
  for (const auto& entry : entries_) {
    top += WifiMenuPanel::WifiMenuEntry::kLineHeight;
    if (at.y > bottom && at.y < top) {
      std::get<0>(entry)->NotifyHoverMouseEnter();
    } else {
      std::get<0>(entry)->NotifyHoverMouseExit();
    }
    bottom = top;
  }
  win.Repaint();
}

void WifiMenuPanel::WifiMenuSection::OnClick(xpp::ui::WindowProxy win,
                                             xpp::ui::MouseButton button,
                                             xpp::ui::ButtonAction action,
                                             xpp::gfx::Coord at) {
  ssize_t bottom = 0;
  const auto& entry = GetEntryAt(at, &bottom);
  if (bottom != -1)
    std::get<1>(entry)->NotifyClick(win, button, action, {at.x, at.y - bottom});
}

const std::tuple<WifiMenuPanel::WifiMenuEntry*, xpp::ui::FramePtr>&
WifiMenuPanel::WifiMenuSection::GetEntryAt(xpp::gfx::Coord at,
                                           ssize_t* bottom) {
  static auto empty =
      std::make_tuple<WifiMenuPanel::WifiMenuEntry*, xpp::ui::FramePtr>(
          nullptr, nullptr);
  *bottom = 0;
  ssize_t top = *bottom;
  for (const auto& entry : entries_) {
    top += WifiMenuPanel::WifiMenuEntry::kLineHeight;
    if (at.y > *bottom && at.y < top)
      return entry;
    *bottom = top;
  }
  *bottom = -1;
  return empty;
}

}  // namespace iwgui