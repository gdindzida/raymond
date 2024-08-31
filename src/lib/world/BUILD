cc_library(
    name = "world",
    srcs = [
        "src/hittable_list.cpp",
        "src/material.cpp",
        "src/sphere.cpp",
    ],
    hdrs = [
        "hit_record.hpp",
        "hittable.hpp",
        "hittable_list.hpp",
        "material.hpp",
        "sphere.hpp",
    ],
    visibility = ["//src:__subpackages__"],
    deps = [
        "//src/lib/image",
        "//src/lib/math",
    ],
)
