cc_library(
    name = "image",
    srcs = [
        "src/color.cpp",
        "src/ray.cpp",
    ],
    hdrs = [
        "color.hpp",
        "ray.hpp",
    ],
    visibility = ["//src:__subpackages__"],
    deps = [
        "//src/lib/math",
    ],
)

cc_library(
    name = "camera",
    srcs = [
        "src/camera.cpp",
    ],
    hdrs = [
        "camera.hpp",
    ],
    visibility = ["//src:__subpackages__"],
    deps = [
        "//src/lib/math",
        "//src/lib/multithreading:openmp",
        "//src/lib/world",
    ],
)
