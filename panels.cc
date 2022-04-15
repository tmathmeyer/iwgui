#include "panels.h"

namespace iwgui {

WifiMenuPanel::WifiMenuPanel(std::shared_ptr<base::dbus::Connection> connection)
    : dbus_connection_(std::move(connection)) {
  station_ = dbus_connection_->GetInterface<iwd::Station>(
      "net.connman.iwd", "/net/connman/iwd/0/4");
  Reset();
}

const xpp::component::Container::Packs& WifiMenuPanel::ContainerPacks(
    const xpp::ui::WindowProxy& w) {
  static xpp::component::Container::Packs packs;
  constexpr ssize_t margin = 12;

  if (!packs.empty())
    packs.clear();

  xpp::gfx::Coord top_left = {margin / 2, margin / 2};
  for (const auto& section : sections_) {
    xpp::gfx::Coord tlc = top_left;
    uint32_t section_height = std::get<0>(section)->Height();
    packs.push_back(
        std::make_tuple<xpp::ui::Frame*, xpp::gfx::Coord, xpp::gfx::Rect>(
            std::get<1>(section).get(), std::move(tlc),
            {w.GetFrameSize().width, section_height}));
    top_left = {top_left.x, top_left.y + section_height + margin};
  }

  return packs;
}

xpp::gfx::Rect WifiMenuPanel::GetPreferredSize() {
  uint32_t margin = 12;
  uint32_t height = margin / 2;
  for (const auto& section : sections_) {
    height += std::get<0>(section)->Height();
    height += margin;
  }
  return {500, height};
}

WifiMenuPanel::WifiMenuSection* WifiMenuPanel::GetSection(size_t index) {
  while (index + 1 > sections_.size()) {
    std::tuple<WifiMenuPanel::WifiMenuSection*, xpp::ui::FramePtr> new_frame =
        xpp::ui::FrameFactory<WifiMenuPanel::WifiMenuSection>::CaptureFrame(
            this);
    sections_.push_back(std::move(new_frame));
  }
  return std::get<0>(sections_[index]);
}

void WifiMenuPanel::OnPaint(xpp::ui::WindowProxy win, xpp::gfx::Graphics g) {
  g.SetColor(xpp::gfx::SDRColor::GRAY8);
  g.FillRect(0, 0, g.size().width, g.size().height);
  xpp::component::Container::PropagateDraw(ContainerPacks(win), win, g);
}

bool WifiMenuPanel::OnMouseMotion(xpp::ui::MotionEvent e) {
  return xpp::component::Container::PropagateMotion(ContainerPacks(e.Window()),
                                                    std::move(e));
}

void WifiMenuPanel::OnMouseEntered(xpp::ui::EnterEvent e) {
  xpp::component::Container::PropagateEntry(ContainerPacks(e.Window()),
                                            std::move(e));
}

void WifiMenuPanel::OnMouseExited(xpp::ui::ExitEvent e) {
  xpp::component::Container::PropagateExit(ContainerPacks(e.Window()),
                                           std::move(e));
  e.Window().Close();
}

bool WifiMenuPanel::OnClick(xpp::ui::ClickEvent e) {
  return xpp::component::Container::PropagateClick(ContainerPacks(e.Window()),
                                                   std::move(e));
}

void WifiMenuPanel::Reset() {
  auto* section = GetSection(1);
  section->Erase();
  station_->Scan();
  for (auto& net : station_->GetOrderedNetworks()) {
    if (net)
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
  g.DrawText(20, 0, displayed_network_->Name());
  if (displayed_network_->Connected()) {
    g.FillRect(7, 27, 6, 6);
  }
}

bool WifiMenuPanel::WifiMenuEntry::OnMouseMotion(xpp::ui::MotionEvent e) {
  return false;
}

bool WifiMenuPanel::WifiMenuEntry::OnClick(xpp::ui::ClickEvent e) {
  if (e.Action() != xpp::ui::ButtonAction::kPress)
    return true;
  std::cout << displayed_network_->Name() << "\n";
  return true;
  /*
  if (e.Action() == xpp::ui::ButtonAction::kPress) {
    pressed_ = true;
  } else if (pressed_ && !displayed_network_->Connected()) {
    pressed_ = false;
    if (displayed_network_->GetKnownNetwork().has_value()) {
      displayed_network_->Connect();
      menu_panel_weak_ref_->Reset();
      e.Window().Repaint();
    } else {
      std::cout << displayed_network_->Name() << " is not a known network\n";
    }
  }
  return true;
  */
}

void WifiMenuPanel::WifiMenuEntry::OnMouseEntered(xpp::ui::EnterEvent e) {
  hovered_ = true;
  pressed_ = false;
  e.Window().Repaint();
}

void WifiMenuPanel::WifiMenuEntry::OnMouseExited(xpp::ui::ExitEvent e) {
  hovered_ = false;
  pressed_ = false;
  e.Window().Repaint();
}

WifiMenuPanel::WifiMenuSection::WifiMenuSection(WifiMenuPanel* menu_weak_ref)
    : menu_panel_weak_ref_(menu_weak_ref) {}

const xpp::component::Container::Packs&
WifiMenuPanel::WifiMenuSection::ContainerPacks(const xpp::ui::WindowProxy& w) {
  static xpp::component::Container::Packs packs;
  if (!packs.empty())
    packs.clear();

  xpp::gfx::Coord top_left = {0, 0};
  for (const auto& entry : entries_) {
    xpp::gfx::Coord tlc = top_left;
    packs.push_back(
        std::make_tuple<xpp::ui::Frame*, xpp::gfx::Coord, xpp::gfx::Rect>(
            std::get<1>(entry).get(), std::move(tlc),
            {w.GetFrameSize().width,
             WifiMenuPanel::WifiMenuEntry::kLineHeight}));
    top_left = {top_left.x,
                top_left.y + WifiMenuPanel::WifiMenuEntry::kLineHeight};
  }

  return packs;
}

void WifiMenuPanel::WifiMenuSection::Erase() {
  entries_.clear();
}

uint32_t WifiMenuPanel::WifiMenuSection::Height() {
  return WifiMenuPanel::WifiMenuEntry::kLineHeight * entries_.size();
}

WifiMenuPanel::WifiMenuEntry* WifiMenuPanel::WifiMenuSection::CreateNextEntry(
    std::unique_ptr<iwd::Network> network) {
  entries_.push_back(
      xpp::ui::FrameFactory<WifiMenuPanel::WifiMenuEntry>::CaptureFrame(
          std::move(network), menu_panel_weak_ref_));
  return std::get<0>(entries_[entries_.size() - 1]);
}

void WifiMenuPanel::WifiMenuSection::OnPaint(xpp::ui::WindowProxy win,
                                             xpp::gfx::Graphics g) {
  xpp::component::Container::PropagateDraw(ContainerPacks(win), win, g);
}

bool WifiMenuPanel::WifiMenuSection::OnMouseMotion(xpp::ui::MotionEvent e) {
  return xpp::component::Container::PropagateMotion(ContainerPacks(e.Window()),
                                                    std::move(e));
}

void WifiMenuPanel::WifiMenuSection::OnMouseEntered(xpp::ui::EnterEvent e) {
  xpp::component::Container::PropagateEntry(ContainerPacks(e.Window()),
                                            std::move(e));
}

void WifiMenuPanel::WifiMenuSection::OnMouseExited(xpp::ui::ExitEvent e) {
  xpp::component::Container::PropagateExit(ContainerPacks(e.Window()),
                                           std::move(e));
}

bool WifiMenuPanel::WifiMenuSection::OnClick(xpp::ui::ClickEvent e) {
  return xpp::component::Container::PropagateClick(ContainerPacks(e.Window()),
                                                   std::move(e));
}

}  // namespace iwgui