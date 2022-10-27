#pragma once

#include "../iwd/types.h"
#include "xpp/ui/panel.h"
#include "xpp/ui/scroll_panel.h"

namespace iwgui {

class MainPanel : public xpp::ui::XScrollPanel {
 public:
  MainPanel(std::shared_ptr<base::dbus::Connection> connection);
  void Paint(xpp::ui::Graphics* g) override;

 private:
  std::shared_ptr<base::dbus::Connection> connection_;
};

}  // namespace iwgui

