project('fejix_headers')

add_languages('c')

# dict[interface_id:str -> versions:list[str]]
interfaces = {
    'app': ['0.0.1'],
    'app_manual_sleep': ['0.0.1'],
    'ram': ['0.0.1'],
    # 'opengl': ['0.0.1'],
}

headers = [
    'fejix'/'core'/'alloc.h',
    'fejix'/'core'/'base.h',
    'fejix'/'core'/'utils.h',
    'fejix'/'core'/'vec.h',
    'fejix'/'loader'/'loader.h',
]
foreach interface, versions: interfaces
    headers += 'fejix'/'interface'/interface+'.h'
endforeach

headers_dep = declare_dependency(include_directories: '.')
install_headers(headers, preserve_path: true)
