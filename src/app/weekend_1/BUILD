cc_binary(
    name = "hello_world",
    srcs = ["hello_world.cpp"],
    deps = [
        "//src/lib/geometry:vec3",
        "//src/lib/types:the_types",
    ],
)

cc_binary(
    name = "raymond",
    srcs = ["raymond.cpp"],
    deps = [
        "//src/lib/geometry:my_math",
        "//src/lib/geometry:ray",
        "//src/lib/types:the_types",
    ],
)

cc_binary(
    name = "raymond2",
    srcs = ["raymond2.cpp"],
    deps = [
        "//src/lib/geometry:hittable",
        "//src/lib/geometry:interval",
        "//src/lib/geometry:my_math",
        "//src/lib/geometry:ray",
        "//src/lib/types:the_types",
    ],
)
