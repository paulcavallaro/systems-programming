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
