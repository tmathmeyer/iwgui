langs("C")

cpp_header (
  name = "include",
  srcs = [ "types.h" ],
  deps = [
    "//base/dbus:include",
    "//base/json:include",
  ],
)

cc_object (
  name = "types",
  srcs = [
    "types.cc",
  ],
  includes = [
    ":include",
  ],
  deps = [
    "//base/dbus:dbus",
    "//base/json:json",
  ],
)