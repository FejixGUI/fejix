project('fejix_core')

add_languages('c')

headers_dep = subproject('headers').get_variable('headers_dep')

sources = [
    'alloc.c',
    'base.c',
    'utils.c',
    'vec.c',
]

core = static_library(
    'fejix_core',
    sources: sources,
    dependencies: [headers_dep],
    build_by_default: false,
)

core_dep = declare_dependency(
    link_with: core,
    dependencies: [headers_dep]
)
