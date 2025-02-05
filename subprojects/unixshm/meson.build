if target_machine.system() in ['linux']
    # Enables Linux extensions like memfd_create in <sys/mman.h> (not needed for FreeBSD)
    linux_arguments = [
        '_GNU_SOURCE'
    ]

    add_project_arguments(linux_arguments, language: ['c'])

endif
