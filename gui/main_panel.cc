
#include "main_panel.h"

#include "ssid_entry.h"
#include "station_panel.h"
#include "xpp/ui/button_listener.h"

namespace iwgui {

class MainPanelExitListener : public xpp::ui::MouseMotionListener {
 public:
  void MouseEntered(xpp::ui::MouseMotionEvent*) override {}
  void MouseMoved(xpp::ui::MouseMotionEvent*) override {}
  void MouseDragged(xpp::ui::MouseMotionEvent*) override {}
  void MouseExited(xpp::ui::MouseMotionEvent* e) override {
    e->component->Window()->Close();
  }
};

MainPanel::MainPanel(std::shared_ptr<base::dbus::Connection> connection)
    : xpp::ui::XPanel(), connection_(std::move(connection)) {
  this->AddMouseMotionListener(std::make_unique<MainPanelExitListener>());
  this->AddComponent(
      std::make_unique<StationPanel>(connection_->GetInterface<iwd::Station>(
          "net.connman.iwd", "/net/connman/iwd/0/4")));
}

void MainPanel::Paint(xpp::ui::Graphics* g) {
  xpp::ui::XPanel::Paint(g);
}

std::optional<xpp::gfx::Rect> MainPanel::GetPreferredSize() const {
  return std::nullopt;
}

}  // namespace iwgui