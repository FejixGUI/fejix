/* Automatically generated by tools/autogen.py, do not edit! */

#include <src/loader.h>

#include <fejix/interface/implementation.h>

fj_loader_function_t _fj_implementation_get_id_ptr;
void fj_implementation_get_id(fj_implementation_id_t * out_id)
{
    if (_fj_implementation_get_id_ptr != NULL) {
        ((void(*)(fj_implementation_id_t * out_id))_fj_implementation_get_id_ptr)(out_id);
    }
    /* do nothing by default */;
}

fj_loader_function_t _fj_implementation_get_name_ptr;
void fj_implementation_get_name(char const ** out_name)
{
    if (_fj_implementation_get_name_ptr != NULL) {
        ((void(*)(char const ** out_name))_fj_implementation_get_name_ptr)(out_name);
    }
    /* do nothing by default */;
}

fj_loader_function_t _fj_implementation_get_version_ptr;
void fj_implementation_get_version(fj_version_t * out_version)
{
    if (_fj_implementation_get_version_ptr != NULL) {
        ((void(*)(fj_version_t * out_version))_fj_implementation_get_version_ptr)(out_version);
    }
    /* do nothing by default */;
}

