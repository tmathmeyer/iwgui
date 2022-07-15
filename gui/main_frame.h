#pragma once

#include "../iwd/types.h"

#include "xpp/gfx/graphics.h"
#include "xpp/ui/container/container_utils.h"
#include "xpp/ui/event.h"
#include "xpp/ui/frame.h"
#include "xpp/ui/window_proxy.h"

namespace iwgui {

class MainFrame {
 public:
  void OnPaint(xpp::ui::WindowProxy, xpp::gfx::Graphics);
  bool OnMouseMotion(xpp::ui::MotionEvent);
  void OnMouseEntered(xpp::ui::EnterEvent);
  void OnMouseExited(xpp::ui::ExitEvent);
  bool OnClick(xpp::ui::ClickEvent);
  bool OnScroll(xpp::ui::ScrollEvent);
  std::optional<xpp::gfx::Rect> GetPreferredSize();

 private:
  friend struct xpp::ui::FrameFactory<MainFrame>;
  MainFrame(std::shared_ptr<base::dbus::Connection>);

  const xpp::container::Container::Layout& GetLayout(
      const xpp::ui::WindowProxy&);
  bool IsLayoutStale();

  std::shared_ptr<base::dbus::Connection> connection_;
  xpp::ui::FramePtr device_frame_;
  xpp::ui::FramePtr station_frame_;
  xpp::ui::FramePtr line_break_;
  xpp::container::Container::Layout layout_;
};

}  // namespace iwgui