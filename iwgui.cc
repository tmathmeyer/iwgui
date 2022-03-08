
#include "base/args/argparse.h"
#include "base/dbus/dbus.h"
#include "base/tracing/trace.h"
#include "panels.h"
#include "xpp/ui/window.h"

int main(int argc, char** argv) {
  auto mons = xpp::ui::XDisplay::GetMonitorSizes();
  if (mons.size() == 0)
    return 1;

  auto dimensions = mons.begin()->second;
  auto window =
      xpp::ui::Window::Create(xpp::ui::Window::Positioning::kFixedPosition,
                              xpp::ui::Window::PositionPin::kTopCenter,
                              /*size=*/{400, 1},
                              /*position=*/{dimensions.width / 2, 0});

  window.SetDefaultFrame(xpp::ui::FrameFactory<iwgui::WifiMenuPanel>::MakeFrame(
      base::dbus::Connection::GetSystemConnection()));

  std::move(window).SetVisibleAndStartEventLoop();
}