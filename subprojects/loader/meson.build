project('fejix_loader')

add_languages('c')
add_project_arguments('-DFJ_OPT_INTERNAL', language: 'c')

sources = ['src'/'loader.c']
foreach interface_header_id: subproject('headers').get_variable('interface_header_ids')
    sources += 'src'/'autogen'/interface_header_id+'.c'
endforeach

loader = library(
    'fejix_loader',
    sources: sources,
    include_directories: '.',
    dependencies: [subproject('core').get_variable('core_dep')],
    install: true,
)

loader_dep = declare_dependency(link_with: loader)
