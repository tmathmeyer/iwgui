#pragma once

#include "../iwd/types.h"
#include "xpp/ui/panel.h"

namespace iwgui {

class StationPanel : public xpp::ui::XPanel {
 public:
  StationPanel(std::unique_ptr<iwd::Station> station);
  void Paint(xpp::ui::Graphics* g) override;
  std::optional<xpp::gfx::Rect> GetPreferredSize() const override;

  void Update();

 private:
  std::unique_ptr<iwd::Station> station_;
};

}  // namespace iwgui

