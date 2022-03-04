
#ifndef IWGUI_PANELS_H_
#define IWGUI_PANELS_H_

#include "iwd_types.h"

#include "base/dbus/dbus.h"
#include "xpp/gfx/graphics.h"
#include "xpp/ui/frame.h"

namespace iwgui {

#define DECLARE_FRAME_METHODS()                             \
  void OnPaint(xpp::ui::WindowProxy, xpp::gfx::Graphics);   \
  void OnKey(xpp::ui::WindowProxy, xpp::ui::KeyEvent, int); \
  void OnMotion(xpp::ui::WindowProxy, xpp::gfx::Coord);     \
  void OnClick(xpp::ui::WindowProxy, xpp::ui::MouseButton,  \
               xpp::ui::ButtonAction, xpp::gfx::Coord)

class WifiMenuPanel {
 public:
  class WifiMenuEntry {
   public:
    static constexpr uint32_t kLineHeight = 60;
    DECLARE_FRAME_METHODS();
    void NotifyHoverMouseEnter();
    void NotifyHoverMouseExit();

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

    const std::tuple<WifiMenuEntry*, xpp::ui::FramePtr>& GetEntryAt(
        xpp::gfx::Coord,
        ssize_t*);

    std::vector<std::tuple<WifiMenuEntry*, xpp::ui::FramePtr>> entries_;
    WifiMenuPanel* menu_panel_weak_ref_;
  };

  DECLARE_FRAME_METHODS();
  WifiMenuSection* GetSection(size_t index);
  void Reset();

 private:
  friend struct xpp::ui::FrameFactory<WifiMenuPanel>;
  WifiMenuPanel(std::shared_ptr<base::dbus::Connection>);

  const std::tuple<WifiMenuSection*, xpp::ui::FramePtr>& GetSectionAt(
      xpp::gfx::Coord,
      ssize_t*);

  std::shared_ptr<base::dbus::Connection> dbus_connection_;
  std::unique_ptr<iwd::Station> station_;
  std::vector<std::tuple<WifiMenuSection*, xpp::ui::FramePtr>> sections_;
};

}  // namespace iwgui

#endif  // IWGUI_PANELS_H_