project('fejix_core')

add_languages('c')
add_project_arguments('-DFJ_OPT_INTERNAL', language: 'c')

headers_dep = subproject('headers').get_variable('headers_dep')

sources = [
    'src'/'alloc.c',
    'src'/'base.c',
    'src'/'utils.c',
    'src'/'vec.c',
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
