#pragma once

#include "../iwd/types.h"
#include "xpp/ui/component.h"

namespace iwgui {

class SSIDEntry : public xpp::ui::XComponent {
 public:
  SSIDEntry(std::unique_ptr<iwd::Network> network);
  void Paint(xpp::ui::Graphics* g) override;
  std::optional<uint32_t> GetPreferredHeight() const override;
  std::optional<uint32_t> GetPreferredWidth() const override;

  void Enter();
  void Exit();
  void Press();
  void Release();

 private:
  std::unique_ptr<iwd::Network> network_;
  bool pressed_ = false;
  bool hovered_ = false;
};

}  // namespace iwgui