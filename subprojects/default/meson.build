project('fejix_default')

add_languages('c')

# dict[interface_id:str -> dict[version:str -> file_name:str]]
default_sources = {}
foreach interface, versions: subproject('headers').get_variable('interfaces')
    version_defaults = {}
    foreach version: versions
        version_defaults += {
            version: files('src'/'autogen'/interface+'-since-v'+version+'.c')
        }
    endforeach

    default_sources += {
        version: version_defaults
    }
endforeach
