#include "device_panel.h"

#include <sstream>

#include "ssid_entry.h"
#include "xpp/ui/button_listener.h"

namespace iwgui {

namespace {

class DeviceInfoView : public xpp::ui::XPanel {
 public:
  DeviceInfoView(std::unique_ptr<iwd::Device> device)
      : device_(std::move(device)) {
    if (device_->Powered())
      prev_max_len_ = device_->GetAdapter()->Model().length();
    else
      prev_max_len_ = 11;
  }

  void Paint(xpp::ui::Graphics* g) override {
    g->SetFontSize(8);
    g->SetColor("DevicePropertyTextColor");
    if (device_->Powered()) {
      auto adapter = device_->GetAdapter();
      auto model = adapter->Model();
      auto name = adapter->Name();
      auto vendor = adapter->Vendor();
      g->DrawText({30, 0}, model);
      g->DrawText({30, 40}, name);
      g->DrawText({30, 80}, vendor);
      prev_max_len_ =
          std::max(model.length(), std::max(name.length(), vendor.length()));
    } else {
      prev_max_len_ = 11;
      g->DrawText({30, 0}, "Powered Off");
    }
  }

  std::optional<uint32_t> GetPreferredHeight() override { return 120; }

  std::optional<uint32_t> GetPreferredWidth() override {
    return prev_max_len_ * 8 * 2.42 + 30;
  }

 private:
  std::unique_ptr<iwd::Device> device_;
  uint32_t prev_max_len_ = 0;
};

std::string RenderDeviceLabel(iwd::Device* device) {
  std::stringstream sstream;
  sstream << device->Name() << " (" << device->Address() << ")";
  return sstream.str();
}

}  // namespace

DevicePanel::DevicePanel(std::unique_ptr<iwd::Device> device)
    : xpp::ui::XAccordion(RenderDeviceLabel(device.get()), 10) {
  AddComponent(std::make_unique<DeviceInfoView>(std::move(device)));
}

}  // namespace iwgui