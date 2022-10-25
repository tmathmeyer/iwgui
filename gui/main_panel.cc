
#include "main_panel.h"

#include "device_panel.h"
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
  for (auto& device : iwd::Device::GetAll(connection_)) {
    // Add a device view
    auto station = device->GetAssociatedStation();
    this->AddComponent(std::make_unique<DevicePanel>(std::move(device)));
    this->AddComponent(std::make_unique<StationPanel>(std::move(station)));
  }
}

void MainPanel::Paint(xpp::ui::Graphics* g) {
  xpp::ui::XPanel::Paint(g);
}

std::optional<xpp::gfx::Rect> MainPanel::GetPreferredSize() const {
  return std::nullopt;
}

}  // namespace iwgui