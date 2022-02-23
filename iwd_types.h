
#include <memory>

#include "base/dbus/dbus.h"

#ifndef IWGUI_IWD_TYPES_H_
#define IWGUI_IWD_TYPES_H_

namespace iwd {

class Adapter {
 public:
  static const char* GetTypeName() { return "net.connman.iwd.Adapter"; }
  static std::unique_ptr<Adapter> CreateFromProxy(
    const base::dbus::ObjectProxy& ns);

  Adapter(std::unique_ptr<base::dbus::ObjectProxy> self,
          std::string model,
          std::string name,
          std::string vendor,
          bool powered);

  const std::string Model() { return model_; }
  const std::string Name() { return name_; }
  const std::string Vendor() { return vendor_; }
  bool Powered() { return powered_; }

 private:
  std::string model_;
  std::string name_;
  std::string vendor_;
  bool powered_;
  std::unique_ptr<base::dbus::ObjectProxy> self_;
};


class Device {
 public:
  static const char* GetTypeName() { return "net.connman.iwd.Device"; }
  static std::unique_ptr<Device> CreateFromProxy(
    const base::dbus::ObjectProxy& ns);

  Device(std::unique_ptr<base::dbus::ObjectProxy> self,
          std::string address,
          std::string name,
          bool powered,
          std::unique_ptr<base::dbus::ObjectProxy> adapter);

  std::unique_ptr<Adapter> GetAdapter();
  const std::string& Address() { return address_; }
  const std::string& Name() { return name_; }
  bool Powered() { return powered_; }

 private:
  std::string address_;
  std::string name_;
  bool powered_;
  std::unique_ptr<base::dbus::ObjectProxy> adapter_;
  std::unique_ptr<base::dbus::ObjectProxy> self_;
};


class KnownNetwork {
 public:
  static const char* GetTypeName() { return "net.connman.iwd.KnownNetwork"; }
  static std::unique_ptr<KnownNetwork> CreateFromProxy(
    const base::dbus::ObjectProxy& ns);

  KnownNetwork(std::unique_ptr<base::dbus::ObjectProxy> self,
               std::string type,
               std::string name,
               std::string last_connected_time,
               bool hidden,
               bool auto_connect);

  const std::string& Type() { return type_; }
  const std::string& Name() { return name_; }
  const std::string& LastConnectedTime() { return last_connected_time_; }
  bool AutoConnect() { return auto_connect_; }
  bool Hidden() { return hidden_; }

 private:
  std::string type_;
  std::string name_;
  std::string last_connected_time_;
  bool hidden_;
  bool auto_connect_;
  std::unique_ptr<base::dbus::ObjectProxy> self_;
};


class Network {
 public:
  static const char* GetTypeName() { return "net.connman.iwd.Network"; }
  static std::unique_ptr<Network> CreateFromProxy(
    const base::dbus::ObjectProxy& ns);

  Network(std::unique_ptr<base::dbus::ObjectProxy> self,
          std::string type,
          std::string name,
          bool connected,
          std::unique_ptr<base::dbus::ObjectProxy> known_network,
          std::unique_ptr<base::dbus::ObjectProxy> device);

  std::unique_ptr<Device> GetDevice();
  std::unique_ptr<KnownNetwork> GetKnownNetwork();
  const std::string& Type() { return type_; }
  const std::string& Name() { return name_; }
  bool Connected() { return connected_; }

 private:
  std::string type_;
  std::string name_;
  bool connected_;
  std::unique_ptr<base::dbus::ObjectProxy> known_network_;
  std::unique_ptr<base::dbus::ObjectProxy> device_;
  std::unique_ptr<base::dbus::ObjectProxy> self_;
};


class Station {
 public:
  static const char* GetTypeName() { return "net.connman.iwd.Station"; }
  static std::unique_ptr<Station> CreateFromProxy(
    const base::dbus::ObjectProxy& ns);

  Station(std::unique_ptr<base::dbus::ObjectProxy> self,
          std::string state,
          bool scanning,
          std::unique_ptr<base::dbus::ObjectProxy> network);

  std::unique_ptr<Network> GeConnectedNetwork();
  const std::string& State() { return state_; }
  bool Scanning() { return scanning_; }

  std::vector<std::unique_ptr<Network>> GetOrderedNetworks();

 private:
  std::string state_;
  bool scanning_;
  std::unique_ptr<base::dbus::ObjectProxy> network_;
  std::unique_ptr<base::dbus::ObjectProxy> self_;
};

}  // namespace iwd

#endif  // IWGUI_IWD_TYPES_H_
