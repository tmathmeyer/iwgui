
#include "iwgui/iwd_types.h"

int main() {
  auto conn = base::dbus::Connection::GetSystemConnection();
  if (!conn) {
    puts("No Connection");
    return -1;
  }

  std::unique_ptr<iwd::Station> station = conn->GetInterface<iwd::Station>(
    "net.connman.iwd", "/net/connman/iwd/0/4");

  std::cout << "Connected to: " << station->GeConnectedNetwork()->Name() << "\n";

  for (const auto& net : station->GetOrderedNetworks()) {
    std::cout << "  avail: " << net->Name() << "\n";
  }
}