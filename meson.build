project(
    'fejix', 'c',
    version: '0.0.1',
    license: 'MIT',
    license_files: 'LICENSE.txt',
    meson_version: '>=1.2.0',

    default_options: [
        'c_std=c99',
        'cpp_std=cpp11',
        'warning_level=3',
        'default_library=static'
    ]
)

cc = meson.get_compiler('c')

implementations = get_option('implementations')
features = get_option('features')

if cc.has_argument('-Wunused')
    add_project_arguments(['-Wunused'], language: ['c', 'cpp', 'objc'])
endif

if host_machine.system() in ['windows', 'cygwin']
    add_project_arguments(['-D_CRT_SECURE_NO_WARNINGS', '-DUNICODE', '-D_UNICODE'], language: ['c', 'cpp'])
endif

foreach option : implementations + features
    add_project_arguments(['-DFJ_OPT_' + option.to_upper()], language: ['c', 'cpp', 'objc'])
endforeach

if implementations.length() != 0
    add_project_arguments(['-DFJ_OPT_ANY_IMPLEMENTATION'], language: ['c', 'cpp', 'objc'])
endif

# SOURCES

fejix_src = [
    'src/implementation.c',

    'src/core/any.c',
    'src/core/error.c',
    'src/core/geometry.c',
    'src/core/malloc.c',
    'src/core/map.c',
    'src/core/vec.c',
    'src/core/utils.c',

    'src/ext/helper.c',
]

dependencies = []
shared_modules = []


if 'wayland' in implementations
    dependencies += dependency('wayland-client', required: true)

    shared_modules += ['unixpoll']

    fejix_src += [
        'src/wayland/implementation.c',

        'src/wayland/client/client.c',
    ]
endif


if 'unixpoll' in shared_modules
    fejix_src += [
        'src/unixpoller/unixpoller.c'
    ]
endif


fejix = library(
    'fejix',
    sources: fejix_src,
    dependencies: dependencies,
    include_directories: ['include', '.'],
)

fejix_dep = declare_dependency(
    link_with: fejix,
    include_directories: ['include']
)


# TESTS

if not meson.is_subproject()
    fejix_internal_dep = declare_dependency(
        link_with: fejix,
        include_directories: ['include', '.']
    )

    executable('dev', sources: ['tests/dev.c'], dependencies: fejix_internal_dep)

    test(
        'test_core_map',
        executable(
            'test_core_map',
            sources: ['tests/core/map.c'],
            dependencies: fejix_internal_dep
        )
    )

    test(
        'test_core_vec',
        executable(
            'test_core_vec',
            sources: ['tests/core/vec.c'],
            dependencies: fejix_internal_dep
        )
    )
endif