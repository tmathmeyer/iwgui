#pragma once

#include "../iwd/types.h"
#include "xpp/ui/accordion.h"

namespace iwgui {

class DevicePanel : public xpp::ui::XAccordion {
 public:
  DevicePanel(std::unique_ptr<iwd::Device> device);
  std::string GetTypeName() const { return "DevicePanel"; }
};

}  // namespace iwgui
