#pragma once

#include "../iwd/types.h"

#include "xpp/gfx/graphics.h"
#include "xpp/ui/frame.h"

namespace iwgui {

class DeviceFrame {
 public:
  static xpp::ui::FramePtr Create(std::unique_ptr<iwd::Device>);

  void OnPaint(xpp::ui::WindowProxy, xpp::gfx::Graphics);
  xpp::gfx::Rect GetPreferredSize();

 private:
  friend struct xpp::ui::FrameFactory<DeviceFrame>;
  DeviceFrame(std::unique_ptr<iwd::Device> network);

  std::unique_ptr<iwd::Device> device_;
};

}  // namespace iwgui