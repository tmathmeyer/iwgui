#include "ssid_entry.h"
#include "xpp/ui/button_listener.h"

namespace iwgui {

SSIDEntry::SSIDEntry(std::unique_ptr<iwd::Network> network)
    : XComponent(), network_(std::move(network)) {
  auto button_action =
      std::make_shared<xpp::ui::ButtonListener<SSIDEntry>>(this);
  this->AddMouseMotionListener(button_action);
  this->AddMouseListener(button_action);
}

void SSIDEntry::Paint(xpp::ui::Graphics* g) {
  if (network_->Connected()) {
    if (pressed_)
      g->SetColor("SSIDConnectedTextPressed");
    else if (hovered_)
      g->SetColor("SSIDConnectedTextHovered");
    else
      g->SetColor("SSIDConnectedTextDefault");
  } else {
    if (pressed_)
      g->SetColor("SSIDDisonnectedTextPressed");
    else if (hovered_)
      g->SetColor("SSIDDisonnectedTextHovered");
    else
      g->SetColor("SSIDDisonnectedTextDefault");
  }
  g->SetFont("Fantasque Sans Mono");
  uint16_t fontsize = 8;
  g->SetFontSize(fontsize);
  auto topoffset = (g->GetDimensions().height - g->GetFontHeight()) / 2;
  g->DrawText({20, topoffset}, network_->Name());
}

std::optional<uint32_t> SSIDEntry::GetPreferredHeight() const {
  return 50;
}

std::optional<uint32_t> SSIDEntry::GetPreferredWidth() const {
  return 20 + (network_->Name().length() * 8 * 2.5);
}

void SSIDEntry::Enter() {
  hovered_ = true;
  pressed_ = false;
}

void SSIDEntry::Exit() {
  hovered_ = false;
  pressed_ = false;
}

void SSIDEntry::Press() {
  hovered_ = true;
  pressed_ = true;
}

void SSIDEntry::Release() {
  hovered_ = true;
  pressed_ = false;
}

}  // namespace iwgui