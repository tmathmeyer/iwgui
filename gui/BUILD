langs("C")

cpp_header (
  name = "include",
  srcs = [
    "device_panel.h",
    "main_panel.h",
    "ssid_entry.h",
    "station_panel.h",
  ],
  deps = [
    "//iwgui/iwd:include",
    "//xpp/gfx:include",
  ],
)

cc_object (
  name = "libiwgui",
  srcs = [
    "device_panel.cc",
    "main_panel.cc",
    "ssid_entry.cc",
    "station_panel.cc",
  ],
  includes = [
    ":include",
    "//xpp/ui:include",
    "//xpp/ui/layout:include",
  ],
)
