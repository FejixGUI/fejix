project('fejix_winapi')

add_languages('c')

ntddi_version = get_option('winapi_version')
winnt_version = ntddi_version.split('_')[0]
windows_arguments = [
    '-D_CRT_SECURE_NO_WARNINGS',
    '-DNTDDI_VERSION=NTDDI_VERSION_' + ntddi_version,
    '-D_WIN32_WINNT=_WIN32_WINNT_' + winnt_version,
    '-DUNICODE',
    '-D_UNICODE',
    '-DWIN32_LEAN_AND_MEAN',
]
add_project_arguments(windows_arguments, language: ['c', 'cpp'])

sources = ['utils.c']
foreach header_id, default_source: subproject('default').get_variable('default_sources')
    sources += default_source
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
