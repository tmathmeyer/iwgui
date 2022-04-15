
#ifndef IWGUI_PANELS_H_
#define IWGUI_PANELS_H_

#include "iwd_types.h"

#include "base/dbus/dbus.h"
#include "xpp/gfx/graphics.h"
#include "xpp/ui/component/container_utils.h"
#include "xpp/ui/frame.h"

namespace iwgui {

#define DECLARE_FRAME_METHODS()                           \
  void OnPaint(xpp::ui::WindowProxy, xpp::gfx::Graphics); \
  bool OnMouseMotion(xpp::ui::MotionEvent);               \
  void OnMouseEntered(xpp::ui::EnterEvent);               \
  void OnMouseExited(xpp::ui::ExitEvent);                 \
  bool OnClick(xpp::ui::ClickEvent)

class WifiMenuPanel {
 public:
  class WifiMenuEntry {
   public:
    static constexpr uint32_t kLineHeight = 60;
    DECLARE_FRAME_METHODS();

   private:
    friend struct xpp::ui::FrameFactory<WifiMenuEntry>;
    WifiMenuEntry(std::unique_ptr<iwd::Network> network, WifiMenuPanel*);

    std::unique_ptr<iwd::Network> displayed_network_;
    WifiMenuPanel* menu_panel_weak_ref_;

    bool hovered_ = false;
    bool pressed_ = false;
  };

  class WifiMenuSection {
   public:
    DECLARE_FRAME_METHODS();
    void Erase();
    uint32_t Height();
    WifiMenuEntry* CreateNextEntry(std::unique_ptr<iwd::Network> network);

   private:
    friend struct xpp::ui::FrameFactory<WifiMenuSection>;
    friend class WifiMenuPanel;

    WifiMenuSection(WifiMenuPanel*);

    const xpp::component::Container::Packs& ContainerPacks(
        const xpp::ui::WindowProxy&);

    std::vector<std::tuple<WifiMenuEntry*, xpp::ui::FramePtr>> entries_;
    WifiMenuPanel* menu_panel_weak_ref_;
  };

  DECLARE_FRAME_METHODS();
  xpp::gfx::Rect GetPreferredSize();
  WifiMenuSection* GetSection(size_t index);
  void Reset();

 private:
  friend struct xpp::ui::FrameFactory<WifiMenuPanel>;
  WifiMenuPanel(std::shared_ptr<base::dbus::Connection>);

  const xpp::component::Container::Packs& ContainerPacks(
      const xpp::ui::WindowProxy&);

  std::shared_ptr<base::dbus::Connection> dbus_connection_;
  std::unique_ptr<iwd::Station> station_;
  std::vector<std::tuple<WifiMenuSection*, xpp::ui::FramePtr>> sections_;
};

}  // namespace iwgui

#endif  // IWGUI_PANELS_H_