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

if cc.has_argument('-Wunused')
    add_project_arguments(['-Wunused'], language: ['c', 'cpp', 'objc'])
endif

implementations = get_option('implementations')
features = get_option('features')

all_options = implementations + features

needs_windows_header_setup = host_machine.system() in ['windows', 'cygwin']

has_any_implementation = implementations.length() != 0

foreach option : all_options
    add_project_arguments(['-DFJ_OPT_' + option.to_upper()], language: ['c', 'cpp', 'objc'])
endforeach

if has_any_implementation
    add_project_arguments(['-DFJ_OPT_ANY_IMPLEMENTATION'], language: ['c', 'cpp', 'objc'])
endif

if needs_windows_header_setup
    add_project_arguments(['-D_CRT_SECURE_NO_WARNINGS', '-DUNICODE', '-D_UNICODE'], language: ['c', 'cpp'])
endif

# SOURCES

public_includes = ['include']
private_includes = ['include', '.']

dependencies = []
shared_modules = []

sources = [
    'src/implementation.c',
    'src/core/any.c',
    'src/core/error.c',
    'src/core/malloc.c',
    'src/core/map.c',
    'src/core/vec.c',
    'src/core/utils.c',
    'src/ext/helper.c',
]


if 'wayland' in implementations
    dependencies += dependency('wayland-client', required: true)

    shared_modules += ['unixpoller']

    sources += [
        'src/wayland/implementation.c',
        'src/wayland/client/client.c',
    ]
endif


if 'unixpoller' in shared_modules
    sources += [
        'src/unixpoller/unixpoller.c'
    ]
endif


fejix = library(
    'fejix',
    sources: sources,
    dependencies: dependencies,
    include_directories: private_includes,
)

fejix_dep = declare_dependency(
    link_with: fejix,
    include_directories: public_includes
)


# TESTS

if get_option('build_tests')
    fejix_internal_dep = declare_dependency(
        link_with: fejix,
        include_directories: private_includes
    )

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