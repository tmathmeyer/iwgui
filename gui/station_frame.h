#pragma once

#include "../iwd/types.h"

#include "xpp/gfx/graphics.h"
#include "xpp/ui/container/container_utils.h"
#include "xpp/ui/event.h"
#include "xpp/ui/frame.h"
#include "xpp/ui/window_proxy.h"

namespace iwgui {

class StationFrame {
 public:
  static xpp::ui::FramePtr Create(std::unique_ptr<iwd::Station>);

  void OnPaint(xpp::ui::WindowProxy, xpp::gfx::Graphics);
  bool OnMouseMotion(xpp::ui::MotionEvent);
  void OnMouseEntered(xpp::ui::EnterEvent);
  void OnMouseExited(xpp::ui::ExitEvent);
  bool OnClick(xpp::ui::ClickEvent);
  bool OnScroll(xpp::ui::ScrollEvent);
  std::optional<xpp::gfx::Rect> GetPreferredSize();

 private:
  friend struct xpp::ui::FrameFactory<StationFrame>;
  StationFrame(std::unique_ptr<iwd::Station> network);

  void Refresh();

  const xpp::container::Container::Layout& GetLayout(
      const xpp::ui::WindowProxy&);
  bool IsLayoutStale();

  xpp::ui::FramePtr ssid_list_;
  xpp::ui::FramePtr title_;
  xpp::ui::FramePtr refresh_button_;
  xpp::ui::FramePtr power_switch_;

  std::unique_ptr<iwd::Station> station_;
  xpp::container::Container::Layout layout_;
};

}  // namespace iwgui