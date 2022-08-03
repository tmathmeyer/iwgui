
#include "ssid_frame.h"

namespace iwgui {

void SSIDFrame::OnPaint(xpp::ui::WindowProxy w, xpp::gfx::Graphics g) {
  g.SetColor(w.WindowLaF()->FrameDefaultColor);
  g.FillRect(0, 0, g.size().width, g.size().height);
  if (hovered_) {
    g.SetColor(w.WindowLaF()->TextButtonMouseoverColor);
  } else {
    g.SetColor(w.WindowLaF()->TextButtonColor);
  }
  g.SetFont("Fantasque Sans Mono", 8);
  g.DrawText(20, 0, displayed_network_->Name());
  if (displayed_network_->Connected()) {
    g.FillRect(7, 24, 6, 6);
  }
}

bool SSIDFrame::OnMouseMotion(xpp::ui::MotionEvent) {
  return false;
}

void SSIDFrame::OnMouseEntered(xpp::ui::EnterEvent e) {
  hovered_ = true;
  e.Window().Repaint();
}

void SSIDFrame::OnMouseExited(xpp::ui::ExitEvent e) {
  hovered_ = false;
  e.Window().Repaint();
}

bool SSIDFrame::OnClick(xpp::ui::ClickEvent) {
  puts("CLICK");
  return false;
}

xpp::gfx::Rect SSIDFrame::GetPreferredSize() {
  return {500, SSIDFrame::kLineHeight};
}

SSIDFrame::SSIDFrame(std::unique_ptr<iwd::Network> network)
    : displayed_network_(std::move(network)) {}

}  // namespace iwgui