
#include "station_frame.h"
#include "ssid_frame.h"

#include "xpp/ui/component/button.h"
#include "xpp/ui/component/text_panel.h"
#include "xpp/ui/container/stack_panel.h"

namespace iwgui {

namespace {

xpp::ui::FramePtr Station2Frames(iwd::Station* s) {
  std::vector<xpp::ui::FramePtr> result;
  for (auto& net : s->GetOrderedNetworks()) {
    if (net)
      result.push_back(
          xpp::ui::FrameFactory<SSIDFrame>::MakeFrame(std::move(net)));
  }
  return xpp::ui::FrameFactory<xpp::container::StackPanel>::MakeFrame(
      xpp::container::StackPanel::Direction::kVertical, std::move(result));
}

}  // namespace

xpp::ui::FramePtr StationFrame::Create(std::unique_ptr<iwd::Station> d) {
  return xpp::ui::FrameFactory<StationFrame>::MakeFrame(std::move(d));
}

void StationFrame::OnPaint(xpp::ui::WindowProxy w, xpp::gfx::Graphics g) {
  g.SetColor(w.WindowLaF()->FrameDefaultColor);
  g.FillRect(0, 0, g.size().width, g.size().height);
  xpp::container::Container::PropagateDraw(GetLayout(w), w, g);
}

void StationFrame::OnMouseExited(xpp::ui::ExitEvent e) {
  xpp::container::Container::PropagateExit(GetLayout(e.Window()), std::move(e));
}

void StationFrame::OnMouseEntered(xpp::ui::EnterEvent e) {
  xpp::container::Container::PropagateEntry(GetLayout(e.Window()),
                                            std::move(e));
}

bool StationFrame::OnMouseMotion(xpp::ui::MotionEvent e) {
  return xpp::container::Container::PropagateMotion(GetLayout(e.Window()),
                                                    std::move(e));
}

bool StationFrame::OnClick(xpp::ui::ClickEvent e) {
  return xpp::container::Container::PropagateClick(GetLayout(e.Window()),
                                                   std::move(e));
}

bool StationFrame::OnScroll(xpp::ui::ScrollEvent e) {
  return xpp::container::Container::PropagateScroll(GetLayout(e.Window()),
                                                    std::move(e));
}

std::optional<xpp::gfx::Rect> StationFrame::GetPreferredSize() {
  auto size = ssid_list_->GetPreferredSize().value();
  return xpp::gfx::Rect{size.width + 20, size.height + 60};
}

bool StationFrame::IsLayoutStale() {
  return true;
}

const xpp::container::Container::Layout& StationFrame::GetLayout(
    const xpp::ui::WindowProxy& w) {
  if (IsLayoutStale()) {
    layout_.clear();

    uint32_t refresh_width = refresh_button_->GetPreferredSize()->width;
    uint32_t power_width = power_switch_->GetPreferredSize()->width;

    layout_.push_back(xpp::container::FrameRenderSpace{
        refresh_button_.get(),
        {w.GetFrameSize().width - power_width - refresh_width, 0},
        {refresh_width, 60}});

    layout_.push_back(xpp::container::FrameRenderSpace{
        title_.get(),
        {20, 0},
        {w.GetFrameSize().width - power_width - refresh_width - 20, 60}});

    layout_.push_back(xpp::container::FrameRenderSpace{
        ssid_list_.get(),
        {20, 60},
        {w.GetFrameSize().width - 20, ssid_list_->GetPreferredSize()->height}});
  }
  return layout_;
}



StationFrame::StationFrame(std::unique_ptr<iwd::Station> station)
    : ssid_list_(Station2Frames(station.get())),
      title_(xpp::component::TextPanel::Create("Networks", 8)),
      refresh_button_(xpp::component::Button::Create("reload")),
      power_switch_(xpp::component::TextPanel::Create("P", 8)),
      station_(std::move(station)) {
  //refresh_button_->SetClickEvent(base::BindRepeating(&StationFrame::Refresh, this));
}

}  // namespace iwgui