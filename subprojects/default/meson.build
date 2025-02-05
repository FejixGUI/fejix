project('fejix_default')

add_languages('c')

# dict[interface_header_id:str -> default_file:list[File]]
default_sources = {}
foreach interface_header_id: subproject('headers').get_variable('interface_header_ids')
    default_sources += {
        interface_header_id: files('autogen'/interface_header_id+'.c')
    }
endforeach
