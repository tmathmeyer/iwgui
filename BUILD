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

cpp_header (
  name = "panels_header",
  srcs = [ "panels.h" ],
  deps = [
    ":iwd_types_h",
    "//base/dbus:dbus_headers",
    "//xpp/gfx:graphics_header",
    "//xpp/ui:ui_include",
    "//xpp/ui/component:component_include",
  ]
)

cpp_object (
  name = "panels",
  srcs = [ "panels.cc" ],
  deps = [
    ":iwd_types",
    ":panels_header",
    "//xpp/ui/component:container_utils",
  ],
)

cpp_binary (
  name = "iwgui",
  srcs = [ "iwgui.cc" ],
  deps = [
    ":panels",
    ":panels_header",
    "//base/args:argparse",
    "//base/dbus:dbus",
    "//base/dbus:dbus_headers",
    "//base/tracing:trace_h",
    "//base/tracing:tracing",
    "//xpp/ui:ui_include",
    "//xpp/ui:window",
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