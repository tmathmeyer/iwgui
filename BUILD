langs("C")

cpp_header (
  name = "iwd_types_h",
  srcs = [ "iwd_types.h" ],
)

cpp_object (
  name = "iwd_types",
  srcs = [
    "iwd_types.cc",
  ],
  deps = [
    ":iwd_types_h",
    "//base/dbus:dbus",
    "//base/dbus:dbus_headers",
  ],
  include_dirs = [
    "/usr/lib/dbus-1.0/include",
    "/usr/include/dbus-1.0",
    "/usr/include/glib-2.0",
    "/usr/lib/glib-2.0/include",
  ],
  flags = [
    "-ldbus-glib-1",
    "-ldbus-1",
    "-lglib-2.0",
  ],
)

cpp_binary (
  name = "printer",
  srcs = [ "printer.cc" ],
  deps = [
    ":iwd_types_h",
    ":iwd_types",
    "//base/dbus:dbus",
    "//base/dbus:dbus_headers",
  ],
  include_dirs = [
    "/usr/lib/dbus-1.0/include",
    "/usr/include/dbus-1.0",
    "/usr/include/glib-2.0",
    "/usr/lib/glib-2.0/include",
  ],
  flags = [
    "-ldbus-glib-1",
    "-ldbus-1",
    "-lglib-2.0",
  ],
)