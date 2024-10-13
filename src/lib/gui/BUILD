cc_library(
    name = "gui",
    srcs = [
        "src/image_renderer.cpp",
        "src/window_renderer.cpp",
    ],
    hdrs = [
        "image_renderer.hpp",
        "renderer.hpp",
        "window_renderer.hpp",
    ],
    copts = ["-fopenmp"],
    linkopts = [
        "-fopenmp",
        "-lSDL2",
        "-lSDL2_image",
    ],
    visibility = ["//src:__subpackages__"],
    deps = [
        "//src/lib/image:camera",
    ],
)
