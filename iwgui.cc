
#include "base/args/argparse.h"
#include "base/dbus/dbus.h"
#include "panels.h"
#include "xpp/ui/window.h"

int main(int argc, char** argv) {
  std::tuple<int, int, int> x_y_w;
  {
    Flag(Example, "--display", "-d",
         "Draw an iwgui window somewhere on the screen");
    NamedType(x, int);
    NamedType(y, int);
    NamedType(w, int);
    Arg(Example, x, y, w);
    auto args = argparse::GetParseTuple<Example>(argc, argv);
    if (!args.has_value()) {
      argparse::DisplayHelp<Example>(std::cout);
      return 1;
    }
    x_y_w = std::move(args).value();
  }

  auto window =
      xpp::ui::Window::Create(xpp::ui::Window::Positioning::kFixedPosition,
                              xpp::ui::Window::PositionPin::kTopLeft,
                              xpp::gfx::Rect(std::get<2>(x_y_w), 1),
                              {std::get<0>(x_y_w), std::get<1>(x_y_w)});

  window.SetDefaultFrame(xpp::ui::FrameFactory<iwgui::WifiMenuPanel>::MakeFrame(
      base::dbus::Connection::GetSystemConnection()));

  std::move(window).SetVisibleAndStartEventLoop();
}