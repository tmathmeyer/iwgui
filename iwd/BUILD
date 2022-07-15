langs("C")

cpp_header (
  name = "types_h",
  srcs = [ "types.h" ],
)

cpp_object (
  name = "types",
  srcs = [
    "types.cc",
  ],
  deps = [
    ":types_h",
    "//base/dbus:dbus",
    "//base/dbus:dbus_headers",
  ],
)