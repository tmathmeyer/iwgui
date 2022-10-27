
#include <memory>

#include "base/dbus/dbus.h"

#ifndef IWGUI_IWD_TYPES_H_
#define IWGUI_IWD_TYPES_H_

namespace iwd {

class Station;

class Adapter {
 public:
  static const char* GetTypeName() { return "net.connman.iwd.Adapter"; }
  static std::unique_ptr<Adapter> CreateFromProxy(
      const base::dbus::ObjectProxy& ns);

  Adapter(base::dbus::ObjectProxy::Storage self,
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
  base::dbus::ObjectProxy::Storage self_;
};

class Device {
 public:
  static std::vector<std::unique_ptr<Device>> GetAll(
      std::shared_ptr<base::dbus::Connection>);
  static const char* GetTypeName() { return "net.connman.iwd.Device"; }
  static std::unique_ptr<Device> CreateFromProxy(
      const base::dbus::ObjectProxy& ns);

  Device(base::dbus::ObjectProxy::Storage self,
         std::string address,
         std::string name,
         bool powered,
         base::dbus::ObjectProxy::Storage adapter);

  std::unique_ptr<Station> GetAssociatedStation();
  std::unique_ptr<Adapter> GetAdapter();
  const std::string& Address() { return address_; }
  const std::string& Name() { return name_; }
  bool Powered() { return powered_; }

 private:
  std::string address_;
  std::string name_;
  bool powered_;
  base::dbus::ObjectProxy::Storage adapter_;
  base::dbus::ObjectProxy::Storage self_;
};

class KnownNetwork {
 public:
  static const char* GetTypeName() { return "net.connman.iwd.KnownNetwork"; }
  static std::unique_ptr<KnownNetwork> CreateFromProxy(
      const base::dbus::ObjectProxy& ns);

  KnownNetwork(base::dbus::ObjectProxy::Storage self,
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

  void Forget() { self_->Call<>("Forget"); }

 private:
  std::string type_;
  std::string name_;
  std::string last_connected_time_;
  bool hidden_;
  bool auto_connect_;
  base::dbus::ObjectProxy::Storage self_;
};

class Network {
 public:
  static const char* GetTypeName() { return "net.connman.iwd.Network"; }
  static std::unique_ptr<Network> CreateFromProxy(
      const base::dbus::ObjectProxy& ns);

  Network(base::dbus::ObjectProxy::Storage self,
          std::string type,
          std::string name,
          bool connected,
          base::dbus::ObjectProxy::Storage device,
          base::dbus::ObjectProxy::MaybeStorage known_network);

  std::unique_ptr<Device> GetDevice();
  std::optional<std::unique_ptr<KnownNetwork>> GetKnownNetwork();
  const std::string& Type() { return type_; }
  const std::string& Name() { return name_; }
  bool Connected() { return connected_; }

  void Connect() { self_->Call<>("Connect"); }

  std::unique_ptr<Network> Refresh();

 private:
  std::string type_;
  std::string name_;
  bool connected_;
  base::dbus::ObjectProxy::MaybeStorage known_network_;
  base::dbus::ObjectProxy::Storage device_;
  base::dbus::ObjectProxy::Storage self_;
};

class Station {
 public:
  static const char* GetTypeName() { return "net.connman.iwd.Station"; }
  static std::unique_ptr<Station> CreateFromProxy(
      const base::dbus::ObjectProxy& ns);

  Station(base::dbus::ObjectProxy::Storage self,
          std::string state,
          bool scanning,
          base::dbus::ObjectProxy::MaybeStorage network);

  std::unique_ptr<Device> GetAssociatedDevice();
  std::optional<std::unique_ptr<Network>> GetConnectedNetwork();
  const std::string& State() { return state_; }
  bool Scanning() { return scanning_; }

  std::vector<std::unique_ptr<Network>> GetOrderedNetworks();
  void Scan() { self_->Call<>("Scan"); }
  void Disconnect() { self_->Call<>("Disconnect"); }

 private:
  std::string state_;
  bool scanning_;
  base::dbus::ObjectProxy::MaybeStorage network_;
  base::dbus::ObjectProxy::Storage self_;
};

}  // namespace iwd

#endif  // IWGUI_IWD_TYPES_H_
