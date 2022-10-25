
#include "base/dbus/dbus.h"
#include "xpp/ui/window.h"
#include "xpp/xlib/xdisplay.h"

#include "gui/main_panel.h"

int main(int argc, char** argv) {
  const auto monitors = xpp::xlib::XDisplay::GetMonitorSizes();
  if (monitors.size() == 0)
    return 1;

  const auto dimensions = monitors.begin()->second;
  auto window =
      xpp::ui::XWindow::Create(xpp::ui::XWindow::WindowType::kDesktopDock,
                               xpp::ui::XWindow::PositionPin::kTopCenter,
                               {800, 500}, {dimensions.width / 2, 0});

  window->GetLookAndFeel()->SetColor("DevicePropertyTextColor",
                                     xpp::gfx::Color::GRAY4);
  window->GetLookAndFeel()->SetColor("SSIDConnectedTextPressed",
                                     xpp::gfx::Color::BLUE.Darker(0.4));
  window->GetLookAndFeel()->SetColor("SSIDConnectedTextHovered",
                                     xpp::gfx::Color::BLUE.Darker(0.7));
  window->GetLookAndFeel()->SetColor("SSIDConnectedTextDefault",
                                     xpp::gfx::Color::BLUE);

  window->GetLookAndFeel()->SetColor("SSIDDisonnectedTextPressed",
                                     xpp::gfx::Color::GRAY2.Darker(0.4));
  window->GetLookAndFeel()->SetColor("SSIDDisonnectedTextHovered",
                                     xpp::gfx::Color::GRAY2.Darker(0.7));
  window->GetLookAndFeel()->SetColor("SSIDDisonnectedTextDefault",
                                     xpp::gfx::Color::GRAY2);

  window->GetLookAndFeel()->SetColor("ScrollbarColor", xpp::gfx::Color::BLUE);

  window->AddComponent(std::make_unique<iwgui::MainPanel>(
      base::dbus::Connection::GetSystemConnection()));
  window->SetVisible(true);
}