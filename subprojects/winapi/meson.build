project('fejix_winapi')

add_languages('c')
add_project_arguments('-DFJ_OPT_INTERNAL', language: 'c')

ntddi_version = get_option('winapi_version')
winnt_version = ntddi_version.split('_')[0]
args = [
    '-DWIN32_LEAN_AND_MEAN',
    '-DNTDDI_VERSION=NTDDI_' + ntddi_version,
    '-D_WIN32_WINNT=_WIN32_WINNT_' + winnt_version,
    '-DUNICODE',
    '-D_UNICODE',
    '-D_CRT_SECURE_NO_WARNINGS',
]
add_project_arguments(args, language: ['c', 'cpp'])

# MSVC does not support VLAs, do not accidentally introduce them with Clang/GCC.
cc = meson.get_compiler('c')
add_project_arguments(cc.get_supported_arguments(['-Werror=vla']), language: ['c'])


sources = ['src'/'utils.c']

implemented = {
    'app': '<=0.0.1'
}
foreach interface, version_defaults: subproject('default').get_variable('default_sources')
    foreach version, default_source: version_defaults
        if interface not in implemented or not implemented[interface].version_compare(version)
            sources += default_source
        endif
    endforeach
endforeach

winapi = library(
    'fejix_winapi',
    sources: sources,
    include_directories: '.',
    dependencies: [subproject('core').get_variable('core_dep')],
    install: true,
)

winapi_dep = declare_dependency(
    link_with: [winapi]
)
