
#include "types.h"

#include "base/check.h"
#include "base/json/json.h"

namespace iwd {

// static
std::vector<std::unique_ptr<Device>> Device::GetAll(
    std::shared_ptr<base::dbus::Connection> conn) {
  std::vector<std::unique_ptr<Device>> result;
  CHECK(conn != nullptr);
  auto mgr =
      conn->GetInterface<base::dbus::ObjectManager>("net.connman.iwd", "/");
  if (!mgr)
    return result;

  auto objs = mgr->GetManagedObjects();
  if (!objs.has_value())
    return result;

  for (const auto& [path, value] : objs->Values()) {
    auto dict = base::json::Unpack<base::json::Object>(base::json::Copy(value));
    if (!dict.has_value())
      continue;
    if (dict->HasKey("net.connman.iwd.Device"))
      result.push_back(conn->GetInterface<Device>("net.connman.iwd", path));
  }

  return result;
}

// static
std::unique_ptr<Adapter> Adapter::CreateFromProxy(
    const base::dbus::ObjectProxy& ns) {
  return ns.Create<Adapter, std::string, std::string, std::string, bool>(
      "Model", "Name", "Vendor", "Powered");
}

Adapter::Adapter(base::dbus::ObjectProxy::Storage self,
                 std::string model,
                 std::string name,
                 std::string vendor,
                 bool powered) {
  self_ = std::move(self);
  model_ = model;
  name_ = name;
  vendor_ = vendor;
  powered_ = powered;
}

std::unique_ptr<Device> Device::CreateFromProxy(
    const base::dbus::ObjectProxy& ns) {
  return ns.Create<Device, std::string, std::string, bool,
                   typename base::dbus::ObjectProxy::Storage>(
      "Address", "Name", "Powered", "Adapter");
}

Device::Device(base::dbus::ObjectProxy::Storage self,
               std::string address,
               std::string name,
               bool powered,
               base::dbus::ObjectProxy::Storage adapter) {
  self_ = std::move(self);
  address_ = address;
  name_ = name;
  powered_ = powered;
  adapter_ = std::move(adapter);
}

std::unique_ptr<Station> Device::GetAssociatedStation() {
  return Station::CreateFromProxy(*self_);
}

std::unique_ptr<Adapter> Device::GetAdapter() {
  return Adapter::CreateFromProxy(*adapter_);
}

std::unique_ptr<KnownNetwork> KnownNetwork::CreateFromProxy(
    const base::dbus::ObjectProxy& ns) {
  return ns
      .Create<KnownNetwork, std::string, std::string, std::string, bool, bool>(
          "Type", "Name", "LastConnectedTime", "Hidden", "AutoConnect");
}

KnownNetwork::KnownNetwork(base::dbus::ObjectProxy::Storage self,
                           std::string type,
                           std::string name,
                           std::string last_connected_time,
                           bool hidden,
                           bool auto_connect) {
  self_ = std::move(self);
  type_ = type;
  name_ = name;
  last_connected_time_ = last_connected_time;
  hidden_ = hidden;
  auto_connect_ = auto_connect;
}

std::unique_ptr<Network> Network::CreateFromProxy(
    const base::dbus::ObjectProxy& ns) {
  return ns.Create<Network, std::string, std::string, bool,
                   typename base::dbus::ObjectProxy::Storage,
                   typename base::dbus::ObjectProxy::MaybeStorage>(
      "Type", "Name", "Connected", "Device", "KnownNetwork");
}

Network::Network(base::dbus::ObjectProxy::Storage self,
                 std::string type,
                 std::string name,
                 bool connected,
                 base::dbus::ObjectProxy::Storage device,
                 base::dbus::ObjectProxy::MaybeStorage known_network) {
  self_ = std::move(self);
  type_ = type;
  name_ = name;
  connected_ = connected;
  known_network_ = std::move(known_network);
  device_ = std::move(device);
}

std::unique_ptr<Device> Network::GetDevice() {
  return Device::CreateFromProxy(*device_);
}

std::optional<std::unique_ptr<KnownNetwork>> Network::GetKnownNetwork() {
  if (!known_network_.has_value())
    return std::nullopt;
  return KnownNetwork::CreateFromProxy(**known_network_);
}

std::unique_ptr<Station> Station::CreateFromProxy(
    const base::dbus::ObjectProxy& ns) {
  return ns.Create<Station, std::string, bool,
                   typename base::dbus::ObjectProxy::MaybeStorage>(
      "State", "Scanning", "ConnectedNetwork");
}

Station::Station(base::dbus::ObjectProxy::Storage self,
                 std::string state,
                 bool scanning,
                 base::dbus::ObjectProxy::MaybeStorage network) {
  self_ = std::move(self);
  state_ = state;
  scanning_ = scanning;
  network_ = std::move(network);
}

std::unique_ptr<Device> Station::GetAssociatedDevice() {
  return Device::CreateFromProxy(*self_);
}

std::optional<std::unique_ptr<Network>> Station::GetConnectedNetwork() {
  if (!network_.has_value())
    return std::nullopt;
  return Network::CreateFromProxy(**network_);
}

std::vector<std::unique_ptr<Network>> Station::GetOrderedNetworks() {
  using Element = std::tuple<std::unique_ptr<Network>, ssize_t>;

  CHECK(!!this);
  CHECK(!!self_);
  auto entries = self_->Call<std::vector<Element>>("GetOrderedNetworks");
  if (!entries.has_value())
    return {};

  std::vector<std::unique_ptr<Network>> result;
  for (auto& tups : std::move(entries).value()) {
    result.push_back(std::get<0>(std::move(tups)));
  }

  return result;
}

}  // namespace iwd