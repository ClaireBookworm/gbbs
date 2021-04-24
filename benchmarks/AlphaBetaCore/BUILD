cc_library(
  name = "AlphaBetaCore",
  hdrs = ["AlphaBetaCore.h"],
  deps = [
  "//gbbs:gbbs",
  ]
)

cc_binary(
  name = "AlphaBetaCore_main",
  srcs = ["AlphaBetaCore.cc"],
  deps = [":AlphaBetaCore"]
)

package(
  default_visibility = ["//visibility:public"],
)
