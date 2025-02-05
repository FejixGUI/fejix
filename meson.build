project(
    'fejix',
    version: '0.0.1',
    license: 'Apache-2.0',
)


add_languages('c')

foreach arg: ['-Werror=vla']
    if meson.get_compiler('c').has_argument(arg)
        add_project_arguments(arg, language: ['c', 'objc'])
    endif
endforeach

fejix_headers_dep = subproject('headers').get_variable('headers_dep')
fejix_core_dep = subproject('core').get_variable('core_dep')


if get_option('build_implementation')

    implementations = get_option('implementations')

    if implementations == []
        fallback_implementations = {
            'windows': ['winapi'],
            'cygwin': ['winapi'],
            'linux': ['loader', 'wayland', 'x11'],
            'gnu': ['loader', 'wayland', 'x11'],
            'dragonfly': ['loader', 'wayland', 'x11'],
            'freebsd': ['loader', 'wayland', 'x11'],
            'netbsd': ['loader', 'wayland', 'x11'],
            'openbsd': ['loader', 'wayland', 'x11'],
        }

        if target_machine.system() not in fallback_implementations
            error('detecting fejix implementations for this system is not implemented (yet)')
        endif

        implementations = fallback_implementations.get(target_machine.system())
    endif

    if implementations.length() > 1 and 'loader' not in implementations
        implementations += 'loader'
    endif

    default_options = [
        'c_std=c99',
        'objc_std=objc99',
        'cpp_std=c++17',
        'warning_level=3',
    ]

    if 'loader' in implementations
        loader_dep = subproject('loader').get_variable('loader_dep')
        default_options += 'default_library=shared'
    endif

    if 'winapi' in implementations
        winapi_dep = subproject(
            'winapi',
            default_options: default_options
        ).get_variable('winapi_dep')
    endif

endif


# fejix_main_lib_name = implementations[0]
# if implementations.length() > 1
#     fejix_main_lib_name = 'loader'
# endif


# fejix_dep = declare_dependency(
#     link_with: fejix_libs[fejix_main_lib_name],
#     dependencies: [fejix_headers_dep]
# )

