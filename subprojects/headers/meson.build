project('fejix_headers')

add_languages('c')

interface_header_ids = [
    'app',
    'app_manual_sleep',
    'ram',
    'opengl',
]

headers = [
    'fejix'/'core'/'alloc.h',
    'fejix'/'core'/'base.h',
    'fejix'/'core'/'utils.h',
    'fejix'/'core'/'vec.h',
    'fejix'/'loader'/'loader.h',
]
foreach header_id: interface_header_ids
    headers += 'fejix'/'interface'/header_id+'.h'
endforeach

headers_dep = declare_dependency(include_directories: '.')
install_headers(headers, preserve_path: true)
