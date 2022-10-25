#include "device_panel.h"

#include <sstream>

#include "ssid_entry.h"
#include "xpp/ui/button_listener.h"
#include "xpp/ui/layout/accordion_layout.h"

namespace iwgui {

DevicePanel::DevicePanel(std::unique_ptr<iwd::Device> device)
    : xpp::ui::XPanel(), device_(std::move(device)) {
  CHECK(device_);
}

void DevicePanel::Paint(xpp::ui::Graphics* g) {
  xpp::ui::XPanel::Paint(g);
  g->SetFont("Fantasque Sans Mono");
  uint16_t fontsize = 8;
  g->SetFontSize(fontsize);
  g->SetColor("DevicePropertyTextColor");

  std::stringstream sstream;
  sstream << device_->Name() << " (" << device_->Address() << ")";
  g->DrawText({10, 0}, sstream.str());
  if (device_->Powered()) {
    auto adapter = device_->GetAdapter();
    g->DrawText({30, 40}, adapter->Model());
    g->DrawText({30, 80}, adapter->Name());
    g->DrawText({30, 120}, adapter->Vendor());
  } else {
    g->DrawText({30, 40}, "Powered Off");
  }
}

std::optional<xpp::gfx::Rect> DevicePanel::GetPreferredSize() const {
  return xpp::gfx::Rect{750, 160};
}

}  // namespace iwgui