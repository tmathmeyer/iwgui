langs("C")

cc_binary (
  name = "iwgui",
  srcs = [ "iwgui.cc" ],
  includes = [
    "//iwgui/iwd:include",
    "//iwgui/gui:include",
    "//xpp/ui:include",
    "//xpp/xlib:include",
  ],
  deps = [
    "//iwgui/iwd:types",
    "//iwgui/gui:libiwgui",
    "//xpp/ui:xpp-uilib",
  ],
)
