project('fejix_loader')

add_languages('c')
add_project_arguments('-DFJ_OPT_INTERNAL', language: 'c')


deps = [subproject('core').get_variable('core_dep')]
sources = []

if target_machine.system() == 'windows'
    sources += 'src'/'loader-winapi.c'
else
    sources += 'src'/'loader-posix.c'
    deps += dependency('dl')
endif

foreach interface, versions: subproject('headers').get_variable('interfaces')
    sources += 'src'/'autogen'/interface+'.c'
endforeach

loader = library(
    'fejix_loader',
    sources: sources,
    include_directories: '.',
    dependencies: deps,
    install: true,
)

loader_dep = declare_dependency(link_with: loader, compile_args: ['-DFJ_HAS_LOADER'])
