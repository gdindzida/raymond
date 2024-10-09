cc_library(
    name = "world",
    srcs = [
        "src/aabb.cpp",
        "src/bvh_tree.cpp",
        "src/material.cpp",
        "src/sphere.cpp",
        "src/texture.cpp",
    ],
    hdrs = [
        "aabb.hpp",
        "bvh_tree.hpp",
        "hit_record.hpp",
        "hittable.hpp",
        "material.hpp",
        "sphere.hpp",
        "texture.hpp",
    ],
    visibility = ["//src:__subpackages__"],
    deps = [
        "//src/lib/image",
        "//src/lib/math",
    ],
)
