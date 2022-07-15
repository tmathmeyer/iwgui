
#include "base/dbus/dbus.h"
#include "base/tracing/trace.h"
#include "xpp/ui/window.h"

#include "gui/device_frame.h"
#include "gui/station_frame.h"
#include "gui/wifi_frame.h"

int main(int argc, char** argv) {
  auto mons = xpp::ui::XDisplay::GetMonitorSizes();
  if (mons.size() == 0)
    return 1;

  auto dimensions = mons.begin()->second;
  auto window =
      xpp::ui::Window::Create(xpp::ui::Window::WindowType::kDesktopDock,
                              xpp::ui::Window::PositionPin::kTopCenter,
                              /*size=*/{400, 400},
                              /*position=*/{dimensions.width / 2, 0});

  window.SetDefaultFrame(xpp::ui::FrameFactory<iwgui::MainFrame>::MakeFrame(
      base::dbus::Connection::GetSystemConnection()));

  std::move(window).SetVisibleAndStartEventLoop();
}