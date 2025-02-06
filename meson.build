project('fejix', version: '0.0.1', license: 'Apache-2.0')

default_options = [
    'c_std=c99',
    'objc_std=objc99',
    'cpp_std=c++17',
    'warning_level=3',
]


fejix_headers_dep = subproject('headers').get_variable('headers_dep')

fejix_core_dep = subproject('core', default_options: default_options).get_variable('core_dep')


if get_option('build_implementation')

    implementations = get_option('implementations')

    if implementations == []
        implementations = {
            'windows': ['winapi'],
            'cygwin': ['winapi'],
            'linux': ['loader', 'wayland', 'x11'],
            'gnu': ['loader', 'wayland', 'x11'],
            'dragonfly': ['loader', 'wayland', 'x11'],
            'freebsd': ['loader', 'wayland', 'x11'],
            'netbsd': ['loader', 'wayland', 'x11'],
            'openbsd': ['loader', 'wayland', 'x11'],
        }.get(target_machine.system(), [])
    endif

    if implementations == []
        error('detecting fejix implementations for this system is not implemented (yet)')
    endif

    if implementations.length() > 1 and 'loader' not in implementations
        implementations += 'loader'
    endif

    if 'loader' in implementations
        main_implementation_name = 'loader'
    else
        main_implementation_name = implementations[0]
    endif


    if 'loader' in implementations
        loader = subproject('loader', default_options: default_options)
        loader_dep = loader.get_variable('loader_dep')

        main_dep = loader_dep

        default_options += 'default_library=shared'
    endif


    if 'winapi' in implementations
        winapi = subproject('winapi', default_options: default_options)
        winapi_dep = winapi.get_variable('winapi_dep')

        if main_implementation_name == 'winapi'
            main_dep = winapi_dep
        endif
    endif


    fejix_dep = declare_dependency(dependencies: [main_dep, fejix_headers_dep])


endif
