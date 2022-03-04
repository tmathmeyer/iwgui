
#include "iwgui/iwd_types.h"

int main() {
  auto conn = base::dbus::Connection::GetSystemConnection();
  if (!conn) {
    puts("No Connection");
    return -1;
  }

  
  std::unique_ptr<iwd::Station> station = conn->GetInterface<iwd::Station>(
      "net.connman.iwd", "/net/connman/iwd/0/4");

  auto connected = station->GetConnectedNetwork();
  
  std::string name = "disconnected";
  if (connected.has_value())
    name = connected.value()->Name();

  std::cout << "Connected to: " << name << "\n";
  station->Scan();

  for (const auto& net : station->GetOrderedNetworks()) {
    if (!net)
      continue;
    auto known_network = net->GetKnownNetwork();
    if (known_network.has_value())
      std::cout << "  *";
    else
      std::cout << "  ";
    std::cout << net->Name() << "\n";
  }
  
}