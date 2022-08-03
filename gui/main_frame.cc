
#include "main_frame.h"
#include "device_frame.h"
#include "station_frame.h"
#include "xpp/ui/component/line_break_panel.h"

namespace iwgui {

void MainFrame::OnPaint(xpp::ui::WindowProxy w, xpp::gfx::Graphics g) {
  g.SetColor(w.WindowLaF()->FrameDefaultColor);
  g.FillRect(0, 0, g.size().width, g.size().height);
  xpp::container::Container::PropagateDraw(GetLayout(w), w, g);
}

void MainFrame::OnMouseExited(xpp::ui::ExitEvent e) {
  e.Window().Close();
  xpp::container::Container::PropagateExit(GetLayout(e.Window()), std::move(e));
}

void MainFrame::OnMouseEntered(xpp::ui::EnterEvent e) {
  xpp::container::Container::PropagateEntry(GetLayout(e.Window()),
                                            std::move(e));
}

bool MainFrame::OnMouseMotion(xpp::ui::MotionEvent e) {
  return xpp::container::Container::PropagateMotion(GetLayout(e.Window()),
                                                    std::move(e));
}

bool MainFrame::OnClick(xpp::ui::ClickEvent e) {
  return xpp::container::Container::PropagateClick(GetLayout(e.Window()),
                                                   std::move(e));
}

bool MainFrame::OnScroll(xpp::ui::ScrollEvent e) {
  return xpp::container::Container::PropagateScroll(GetLayout(e.Window()),
                                                    std::move(e));
}

std::optional<xpp::gfx::Rect> MainFrame::GetPreferredSize() {
  auto df = device_frame_->GetPreferredSize().value();
  auto sf = station_frame_->GetPreferredSize().value();
  return xpp::gfx::Rect{std::max(df.width, sf.width),
                        21 + df.height + sf.height};
}

bool MainFrame::IsLayoutStale() {
  return true;
}

const xpp::container::Container::Layout& MainFrame::GetLayout(
    const xpp::ui::WindowProxy& w) {
  if (IsLayoutStale()) {
    layout_.clear();

    layout_.push_back(xpp::container::FrameRenderSpace{
        device_frame_.get(),
        {0, 0},
        device_frame_->GetPreferredSize().value()});

    layout_.push_back(xpp::container::FrameRenderSpace{
        line_break_.get(),
        {0, device_frame_->GetPreferredSize()->height},
        {w.GetFrameSize().width, 21}});

    layout_.push_back(xpp::container::FrameRenderSpace{
        station_frame_.get(),
        {0, device_frame_->GetPreferredSize()->height + 21},
        station_frame_->GetPreferredSize().value()});
  }
  return layout_;
}

MainFrame::MainFrame(std::shared_ptr<base::dbus::Connection> connection)
    : connection_(std::move(connection)),
      device_frame_(DeviceFrame::Create(
          connection_->GetInterface<iwd::Device>("net.connman.iwd",
                                                 "/net/connman/iwd/0/4"))),
      station_frame_(StationFrame::Create(
          connection_->GetInterface<iwd::Station>("net.connman.iwd",
                                                  "/net/connman/iwd/0/4"))),
      line_break_(xpp::component::LineBreakPanel::Create(20)) {
  // TODO: determine the device and station ids programatically!
}

}  // namespace iwgui