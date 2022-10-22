#pragma once

#include "../iwd/types.h"
#include "xpp/ui/panel.h"

namespace iwgui {

class MainPanel : public xpp::ui::XPanel {
 public:
  MainPanel(std::shared_ptr<base::dbus::Connection> connection);
  void Paint(xpp::ui::Graphics* g) override;
  std::optional<xpp::gfx::Rect> GetPreferredSize() const override;

 private:
  std::shared_ptr<base::dbus::Connection> connection_;
};

}  // namespace iwgui

