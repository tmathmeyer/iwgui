#include "device_panel.h"

#include "ssid_entry.h"
#include "xpp/ui/button_listener.h"
#include "xpp/ui/layout/accordion_layout.h"

namespace iwgui {

DevicePanel::DevicePanel(std::unique_ptr<iwd::Device> device)
    : xpp::ui::XScrollPanel(), device_(std::move(device)) {
  CHECK(device_);
  Update();
}

void DevicePanel::Update() {
  this->RemoveAll();
  /*
  for (auto& net : device_->GetOrderedNetworks()) {
    if (!net)
      continue;
    this->AddComponent(std::make_unique<SSIDEntry>(std::move(net)));
  }
  */
}

void DevicePanel::Paint(xpp::ui::Graphics* g) {
  xpp::ui::XPanel::Paint(g);
}

}  // namespace iwgui