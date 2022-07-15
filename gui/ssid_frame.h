#pragma once

#include "../iwd/types.h"

#include "xpp/gfx/graphics.h"
#include "xpp/ui/frame.h"

namespace iwgui {

class SSIDFrame {
 public:
  static constexpr uint32_t kLineHeight = 60;

  void OnPaint(xpp::ui::WindowProxy, xpp::gfx::Graphics);
  bool OnMouseMotion(xpp::ui::MotionEvent);
  void OnMouseEntered(xpp::ui::EnterEvent);
  void OnMouseExited(xpp::ui::ExitEvent);
  bool OnClick(xpp::ui::ClickEvent);
  xpp::gfx::Rect GetPreferredSize();

 private:
  friend struct xpp::ui::FrameFactory<SSIDFrame>;
  SSIDFrame(std::unique_ptr<iwd::Network> network);

  std::unique_ptr<iwd::Network> displayed_network_;

  bool hovered_ = false;
  bool pressed_ = false;
};

}  // namespace iwgui