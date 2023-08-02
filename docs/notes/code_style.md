# Coding style

Here are some consistency guidelines.

## General

* Lines should be maximum 80 characters and end with LF.
* Indents should be 4 spaces.
* All files should end with a final newline.
* Use C99.

## Order

### Header files

1. Header guard
2. Header includes
3. Macros
4. Enum constants
5. Structure forward declarations
6. Typedefs
7. Structure definitions
8. Interface definitions
9. Function declarations

Illustration:
```c
#ifndef MY_HEADER_FILE_H_
#define MY_HEADER_FILE_H_

#include <my_include_file.h>

#define MY_MACRO

enum my_enum {
    MY_CONSTANT = 0,
};

struct my_struct_declaration;

typedef .. my_typedef;

struct my_struct {
    ..
};

FJ_DEFINE_INTERFACE(my_interface) {
    ..
};

.. my_function(..);

#endif
```

### Function arguments

1. System
2. Entity ID
3. Event ID
4. Interface ID
5. Data pointers

Illustration:
```c
void func(
    struct fj_sys * sys,
    fj_id_t module_id,
    fj_id_t entity_id,
    fj_id_t event_id,
    fj_ptr_t data
);
```

### Source files

1. Header includes (header being implemented, Fejix headers, dependency headers, standard headers).
2. Static functions
3. Public functions

If a static function is used in exactly one function and is not intended to be used anywhere else, it may come right before the function that uses it.
An example of such static functions is callbacks.

Illustration:
```c
#include <fejix/the_header_that_is_being_implemented.h>
#include <fejix/other.h>

#include <some_lib/some_header.h>

#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>


static .. foo(..)
    ..


static .. bar(..)
    ..


.. fj_some_function(..)
    ..


static .. some_callback(..)
    ..


.. fj_another_function(..)
    .. use some_callback(..) ..
    ..

```
