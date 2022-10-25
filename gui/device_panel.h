#pragma once

#include "../iwd/types.h"
#include "xpp/ui/panel.h"

namespace iwgui {

class DevicePanel : public xpp::ui::XPanel {
 public:
  DevicePanel(std::unique_ptr<iwd::Device> device);
  void Paint(xpp::ui::Graphics* g) override;
  std::optional<xpp::gfx::Rect> GetPreferredSize() const override;

 private:
  std::unique_ptr<iwd::Device> device_;
};

}  // namespace iwgui

