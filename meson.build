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

foreach arg : ['-Wunused', '-Werror=vla']
    if cc.has_argument(arg)
        add_project_arguments(arg, language: ['c'])
    endif
endforeach

implementations = get_option('implementations')
features = get_option('features')

foreach option : implementations
    add_project_arguments(['-DFJ_OPT_IMPLEMENTATION_' + option.to_upper()], language: ['c', 'cpp', 'objc'])
endforeach

foreach option : features
    add_project_arguments(['-DFJ_OPT_FEATURE_' + option.to_upper()], language: ['c', 'cpp', 'objc'])
endforeach

if implementations.length() != 0
    add_project_arguments(['-DFJ_OPT_ANY_IMPLEMENTATION'], language: ['c', 'cpp', 'objc'])
endif

if host_machine.system() in ['windows', 'cygwin']
    add_project_arguments(['-D_CRT_SECURE_NO_WARNINGS', '-DUNICODE', '-D_UNICODE'], language: ['c', 'cpp'])
endif

if host_machine.system() in ['linux']
    add_project_arguments(['-D_GNU_SOURCE'], language: ['c', 'cpp'])
endif


public_includes = ['include']
private_includes = ['include', '.']

dependencies = []
shared_modules = []

sources = [
    'src/core/alloc.c',
    'src/core/base.c',
    'src/core/map.c',
    'src/core/vec.c',
    'src/core/utils.c',

    'src/ext/helper.c',

    'src/generic/implementation.c',
    'src/generic/softer.c',
]


if 'wayland' in implementations
    dependencies += dependency('wayland-client', required: true)

    shared_modules += ['unixpoller']

    sources += [
        'src/wayland/implementation.c',
        'src/wayland/client/client.c',
        'src/wayland/output/output.c',
    ]

    if 'softer_canvas' in features
        sources += [
            'src/wayland/softer/softer.c'
        ]

        shared_modules += ['unixshm']
    endif
endif


if 'winapi' in implementations
    sources += [
        'src/winapi/implementation.c',
        'src/winapi/utils.c',
        'src/winapi/client/client.c',
        'src/winapi/scheduler/scheduler.c',
    ]
endif


if 'unixpoller' in shared_modules
    sources += [
        'src/unixpoller/unixpoller.c'
    ]
endif

if 'unixshm' in shared_modules
    sources += [
        'src/unixshm/unixshm.c'
    ]
endif


fejix = library(
    'fejix',
    sources: sources,
    dependencies: dependencies,
    include_directories: private_includes,
    c_args: ['-DFJ_OPT_INTERNAL']
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

    if 'unixshm_buffer' in shared_modules
        test(
            'test_unixshm',
            executable(
                'test_unixshm',
                sources: ['tests/unixshm/test.c'],
                dependencies: fejix_internal_dep
            )
        )
    endif
endif
