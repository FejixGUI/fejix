# Coding style

Here are some consistency guidelines.

## Standards

* Use C99.

## Formatting

* Use `/*...*/` comments, without any additional "cosmetical" characters.
    ```c
    /* This is a good comment.
        It contains no garbage.
        It is properly indented. */
    void a(void);

    /* This is a bad comment.
     * It is hard to type and edit.
     */
    void a(void);

    /**************************************
     * This is an especially bad comment. *
     **************************************/
    void a(void);

    ```

* Put spaces around `*`:
    ```c
    uint32_t * var;
    uint32_t ** var2;
    uint32_t *** var3;
    uint32_t * function();
    void function(uint32_t * arg);
    ```

* Put `{` on the same line as the signature, except for functions.

    ```c
    enum|struct|union x {
        ...
    };

    uint32_t var[] = {
        ...
    };

    void function()
    {
        ...
    }

    struct some_struct_type long_function(
        struct argument_type ***** some_long_argument,
        const struct argument_type *** another_argument
    )
    {

    }
    ```

## General

* Use `fj_ptr_t` instead of `void *`.

    If there is `void` somewhere in the function return type,
    it must mean that function returns nothing.
    The little star `*` is easy to miss.

* Use `fj_bool_t` instead of `bool`. Use `true`/`false` instead of `1`/`0`.

    `fj_bool_t` has a fixed size, so it is more preferable in public
    declarations.

    We use C99, so `<stdbool.h>` and `true`/`false` are available.

## Order

### Header files

1. Header guard
2. Header includes
3. Macros
4. Enum constants
5. Structure forward declarations
6. Typedefs
7. Structure declarations
8. Interface declarations
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
