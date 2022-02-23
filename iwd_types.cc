
#include "iwgui/iwd_types.h"

namespace iwd {

std::unique_ptr<Adapter> Adapter::CreateFromProxy(
    const base::dbus::ObjectProxy& ns) {
  return ns.Create<Adapter, std::string, std::string, std::string, bool>(
    "Model", "Name", "Vendor", "Powered");
}

Adapter::Adapter(std::unique_ptr<base::dbus::ObjectProxy> self,
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

Device::Device(std::unique_ptr<base::dbus::ObjectProxy> self,
               std::string address,
               std::string name,
               bool powered,
               std::unique_ptr<base::dbus::ObjectProxy> adapter) {
  self_ = std::move(self);
  address_ = address;
  name_ = name;
  powered_ = powered;
  adapter_ = std::move(adapter);
}

std::unique_ptr<Adapter> Device::GetAdapter() {
  return Adapter::CreateFromProxy(*adapter_);
}

std::unique_ptr<KnownNetwork> KnownNetwork::CreateFromProxy(
    const base::dbus::ObjectProxy& ns) {
  return ns.Create<KnownNetwork, std::string, std::string, std::string,
                   bool, bool>(
    "Type", "Name", "LastConnectedTime", "Hidden", "AutoConnect");
}

KnownNetwork::KnownNetwork(std::unique_ptr<base::dbus::ObjectProxy> self,
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
                   typename base::dbus::ObjectProxy::Storage>(
    "Type", "Name", "Connected", "KnownNetwork", "Device");
}

Network::Network(std::unique_ptr<base::dbus::ObjectProxy> self,
                 std::string type,
                 std::string name,
                 bool connected,
                 std::unique_ptr<base::dbus::ObjectProxy> known_network,
                 std::unique_ptr<base::dbus::ObjectProxy> device) {
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

std::unique_ptr<KnownNetwork> Network::GetKnownNetwork() {
  return KnownNetwork::CreateFromProxy(*known_network_);
}

std::unique_ptr<Station> Station::CreateFromProxy(
    const base::dbus::ObjectProxy& ns) {
  return ns.Create<Station, std::string, bool,
                   typename base::dbus::ObjectProxy::Storage> (
    "State", "Scanning", "ConnectedNetwork");
}

Station::Station(std::unique_ptr<base::dbus::ObjectProxy> self,
                 std::string state,
                 bool scanning,
                 std::unique_ptr<base::dbus::ObjectProxy> network) {
  self_ = std::move(self);
  state_ = state;
  scanning_ = scanning;
  network_ = std::move(network);
}

std::unique_ptr<Network> Station::GeConnectedNetwork() {
  return Network::CreateFromProxy(*network_);
}

std::vector<std::unique_ptr<Network>> Station::GetOrderedNetworks() {
  self_->Call("GetOrderedNetworks");

  return {};
}

}  // namespace iwd