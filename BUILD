cc_library(
    name = "cudaImg",
    hdrs = glob(["**/*.hpp"]),
    copts = ["-std=c++17",],
)

cc_test(
    name = "hello_test",
    size = "small",
    srcs = ["hello_test.cc"],
    copts = ["-std=c++17",],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        ":cudaImg",
    ],
)

load("@hedron_compile_commands//:refresh_compile_commands.bzl", rcc="refresh_compile_commands")

rcc(
    name = "rcc",

    # Specify the targets of interest.
    # For example, specify a dict of targets and any flags required to build.
    targets = {
      #"//:my_output_1": "--important_flag1 --important_flag2=true",
      "//...:*": "--cxxopt='-std=c++17'",
    },
    # No need to add flags already in .bazelrc. They're automatically picked up.
    # If you don't need flags, a list of targets is also okay, as is a single target string.
    # Wildcard patterns, like //... for everything, *are* allowed here, just like a build.
      # As are additional targets (+) and subtractions (-), like in bazel query https://docs.bazel.build/versions/main/query.html#expressions
    # And if you're working on a header-only library, specify a test or binary target that compiles it.
)
