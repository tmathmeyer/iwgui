langs("C")

cpp_header (
  name = "ui_headers",
  srcs = [
    "device_frame.h",
    "ssid_frame.h",
    "station_frame.h",
    "main_frame.h",
  ],
  deps = [
    "//iwgui/iwd:types_h",
    "//base/dbus:dbus_headers",
    "//xpp/gfx:graphics_header",
    "//xpp/ui/component:component_include",
  ]
)

cpp_object (
  name = "ssid_frame",
  srcs = [
    "ssid_frame.cc"
  ],
  deps = [
    ":ui_headers",
  ],
)

cpp_object (
  name = "device_frame",
  srcs = [
    "device_frame.cc"
  ],
  deps = [
    ":ui_headers",
  ],
)

cpp_object (
  name = "station_frame",
  srcs = [
    "station_frame.cc"
  ],
  deps = [
    ":ui_headers",
    ":ssid_frame",
    "//xpp/ui/component:text_panel",
    "//xpp/ui/component:stack_panel",
    "//xpp/ui/component:button",
  ],
)

cpp_object (
  name = "main_frame",
  srcs = [
    "main_frame.cc"
  ],
  deps = [
    ":ui_headers",
    ":device_frame",
    ":station_frame",
    "//xpp/ui/component:line_break_panel",
  ],
)