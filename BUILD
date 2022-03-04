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
)



cpp_header (
  name = "panels_header",
  srcs = [ "panels.h" ],
  deps = [
    ":iwd_types_h",
    "//base/dbus:dbus_headers",
    "//xpp/gfx:graphics_header",
    "//xpp/ui:ui_include",
  ]
)

cpp_object (
  name = "panels",
  srcs = [ "panels.cc" ],
  deps = [
    ":iwd_types",
    ":panels_header",
  ],
)

cpp_binary (
  name = "iwgui",
  srcs = [ "iwgui.cc" ],
  deps = [
    ":panels",
    ":panels_header",
    "//base/dbus:dbus",
    "//base/dbus:dbus_headers",
    "//xpp/ui:ui_include",
    "//xpp/ui:window",
  ],
)