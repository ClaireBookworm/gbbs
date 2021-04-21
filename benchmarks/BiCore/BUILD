cc_library(
  name = "BiCore",
  hdrs = ["BiCore.h"],
  deps = [
  "//gbbs:gbbs",
  "//gbbs:julienne",
  ]
)

cc_binary(
  name = "BiCore_main",
  srcs = ["BiCore.cc"],
  deps = [":BiCore"]
)

package(
  default_visibility = ["//visibility:public"],
)
