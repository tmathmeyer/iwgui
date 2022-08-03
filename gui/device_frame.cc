
#include "device_frame.h"

namespace iwgui {

xpp::ui::FramePtr DeviceFrame::Create(std::unique_ptr<iwd::Device> d) {
  return xpp::ui::FrameFactory<DeviceFrame>::MakeFrame(std::move(d));
}

void DeviceFrame::OnPaint(xpp::ui::WindowProxy w, xpp::gfx::Graphics g) {
  g.SetColor(w.WindowLaF()->FrameDefaultColor);
  g.FillRect(0, 0, g.size().width, g.size().height);

  g.SetFont("Fantasque Sans Mono", 8);
  g.SetColor(w.WindowLaF()->TextNormalColor);
  g.DrawText(20, 0, "Wifi");

  g.SetFont("Fantasque Sans Mono", 6);
  g.SetColor(w.WindowLaF()->TextButtonColor);
  g.DrawText(20, 50, "interface: " + device_->Name());
  g.DrawText(20, 90, "address: " + device_->Address());
}

xpp::gfx::Rect DeviceFrame::GetPreferredSize() {
  return {500, 130};
}

DeviceFrame::DeviceFrame(std::unique_ptr<iwd::Device> device)
    : device_(std::move(device)) {}

}  // namespace iwgui