================================================================================
Linking to the library
================================================================================

..  warning::
    If you are using the library as shared (``.dll``) on Microsoft Windows,
    you need to define ``FJ_OPT_DLLIMPORT`` before including any library
    headers so that the library gets linked correctly.
    If you are using the library's CMake target, it will automatically do
    that for you.
