#include "station_panel.h"

#include "ssid_entry.h"
#include "xpp/ui/button_listener.h"
#include "xpp/ui/layout/accordion_layout.h"

namespace iwgui {

StationPanel::StationPanel(std::unique_ptr<iwd::Station> station)
    : xpp::ui::XScrollPanel(), station_(std::move(station)) {
  CHECK(station_);
  Update();
}

void StationPanel::Update() {
  this->RemoveAll();
  for (auto& net : station_->GetOrderedNetworks()) {
    if (!net)
      continue;
    this->AddComponent(std::make_unique<SSIDEntry>(std::move(net)));
  }
}

void StationPanel::Paint(xpp::ui::Graphics* g) {
  xpp::ui::XPanel::Paint(g);
}

std::optional<xpp::gfx::Rect> StationPanel::GetPreferredSize() const {
  return std::nullopt;
}

}  // namespace iwgui