cc_binary(
  name = "hello-world",
  deps = [
    "@com_google_absl//absl/strings",
  ],
  srcs = ["hello-world.cc"],
)

cc_test(
    name = "cache-lines",
    srcs = ["cache-lines.cc"],
    tags = ["benchmark"],
    deps = [
      "@com_github_google_benchmark//:benchmark_main",
      "@com_google_absl//absl/synchronization",
    ],
)

cc_test(
    name = "lock-striping",
    srcs = ["lock-striping.cc"],
    tags = ["benchmark"],
    deps = [
      "@com_github_google_benchmark//:benchmark_main",
      "@com_google_absl//absl/container:flat_hash_set",
      "@com_google_absl//absl/synchronization",
    ],
)

cc_library(
    name = "circular-buffer",
    srcs = ["circular-buffer.h"],
    tags = [],
    deps = [
    ],
)

cc_test(
    name = "circular-buffer-test",
    srcs = ["circular-buffer-test.cc"],
    tags = ["benchmark"],
    deps = [
      ":circular-buffer",
      "@com_github_google_benchmark//:benchmark_main",
    ],
)

cc_test(
    name = "power-of-two",
    srcs = ["power-of-two.cc"],
    tags = ["benchmark"],
    deps = [
      "@com_github_google_benchmark//:benchmark_main",
      "@com_google_absl//absl/random",
    ],
)

cc_test(
    name = "pointer-tagging",
    srcs = ["pointer-tagging.cc"],
    tags = ["benchmark"],
    deps = [
      "@com_github_google_benchmark//:benchmark_main",
      "@com_google_absl//absl/memory",
      "@com_google_googletest//:gtest_main",
    ],
)
