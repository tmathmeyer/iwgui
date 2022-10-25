#pragma once

#include "../iwd/types.h"
#include "xpp/ui/panel.h"
#include "xpp/ui/scroll_panel.h"

namespace iwgui {

class DevicePanel : public xpp::ui::XScrollPanel {
 public:
  DevicePanel(std::unique_ptr<iwd::Device> device);
  void Paint(xpp::ui::Graphics* g) override;

  void Update();

 private:
  std::unique_ptr<iwd::Device> device_;
};

}  // namespace iwgui

