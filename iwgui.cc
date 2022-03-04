
#include "base/dbus/dbus.h"
#include "panels.h"
#include "xpp/ui/window.h"

int main() {
  auto window =
      xpp::ui::Window::Create(xpp::ui::Window::Positioning::kObeyWindowManager,
                              xpp::gfx::Rect(500, 500), xpp::gfx::Coord(0, 0))
          .value();

  window.SetDefaultFrame(xpp::ui::FrameFactory<iwgui::WifiMenuPanel>::MakeFrame(
      base::dbus::Connection::GetSystemConnection()));

  std::move(window).SetVisibleAndStartEventLoop();
}