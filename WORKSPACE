load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Abseil LTS archive version 20210324.2
http_archive(
  name = "com_google_absl",
  urls = ["https://github.com/abseil/abseil-cpp/archive/refs/tags/20210324.2.zip"],
  strip_prefix = "abseil-cpp-20210324.2",
  sha256 = "1a7edda1ff56967e33bc938a4f0a68bb9efc6ba73d62bb4a5f5662463698056c",
)

# glog archive.
http_archive(
  name = "com_github_google_glog",
  urls = ["https://github.com/google/glog/archive/ba8a9f6952d04d1403b97df24e6836227751454e.zip"],
  strip_prefix = "glog-ba8a9f6952d04d1403b97df24e6836227751454e",
  sha256 = "9b4867ab66c33c41e2672b5de7e3133d38411cdb75eeb0d2b72c88bb10375c71",
)

# gflags archive.
http_archive(
  name = "com_github_gflags_gflags",
  strip_prefix = "gflags-2.2.2",
  urls = [
    "https://mirror.bazel.build/github.com/gflags/gflags/archive/v2.2.2.tar.gz",
    "https://github.com/gflags/gflags/archive/v2.2.2.tar.gz",
  ],
)

# googltest archive.
http_archive(
  name = "com_github_google_googletest",
  strip_prefix = "googletest-90a443f9c2437ca8a682a1ac625eba64e1d74a8a",
  urls = ["https://github.com/google/googletest/archive/90a443f9c2437ca8a682a1ac625eba64e1d74a8a.zip"],
  sha256 = "6fb9a49ad77656c860cfdafbb3148a91f076a3a8bda9c6d8809075c832549dd4",
)

# Bazel toolchains
http_archive(
  name = "bazel_toolchains",
  urls = [
    "https://mirror.bazel.build/github.com/bazelbuild/bazel-toolchains/archive/bc09b995c137df042bb80a395b73d7ce6f26afbe.tar.gz",
    "https://github.com/bazelbuild/bazel-toolchains/archive/bc09b995c137df042bb80a395b73d7ce6f26afbe.tar.gz",
  ],
  strip_prefix = "bazel-toolchains-bc09b995c137df042bb80a395b73d7ce6f26afbe",
  sha256 = "4329663fe6c523425ad4d3c989a8ac026b04e1acedeceb56aa4b190fa7f3973c",
)

# GoogleTest/GoogleMock framework. Used by most unit-tests.
http_archive(
  name = "com_google_googletest",
  urls = ["https://github.com/google/googletest/archive/b6cd405286ed8635ece71c72f118e659f4ade3fb.zip"],  # 2019-01-07
  strip_prefix = "googletest-b6cd405286ed8635ece71c72f118e659f4ade3fb",
  sha256 = "ff7a82736e158c077e76188232eac77913a15dac0b22508c390ab3f88e6d6d86",
)

# Google benchmark.
http_archive(
  name = "com_github_google_benchmark",
  urls = ["https://github.com/google/benchmark/archive/16703ff83c1ae6d53e5155df3bb3ab0bc96083be.zip"],
  strip_prefix = "benchmark-16703ff83c1ae6d53e5155df3bb3ab0bc96083be",
  sha256 = "59f918c8ccd4d74b6ac43484467b500f1d64b40cc1010daa055375b322a43ba3",
)
