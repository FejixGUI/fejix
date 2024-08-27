# Coding style

Here are some consistency guidelines.

## Formatting

* Put `{` on its own line ONLY for functions:

    ```c
    struct a {
        // this is not a function
    };

    while (...) {
        // this is not a function
    }

    void f(...)
    {
        // this is a function!
    }

    void f(
        ...
    )
    {
        // this is a function!
    }
    ```

* Put arguments either all on the same line as the function name, or each on its own line:
    ```c
    void f(int aaaaaaaaaaaaaaaaaaaa, int bbbbbbbbbbbbbbbb, int cccccccccccccccccccc)...

    void f(
        int aaaaaaaaaaaaaaaaaaaaaaaaaaaa,
        int bbbbbbbbbbbbbbbbbbbbbbbbbbbb,
        int cccccccccccccccccccccccccccc,
        int dddddddddddddddddddddddddddd
    )...
    ```

* Put each variable declaration on its own line:

    ```c
    // Do not merge these definitions into one line!
    uint32_t a;
    uint32_t b;
    ```

* Put spaces around `*` in types and declarations:
    ```c
    uint32_t * var;
    uint32_t * * var;
    uint32_t * * * var;
    uint32_t *const * var;
    uint32_t * *const var;
    uint32_t const * *const * var;
    uint32_t * function(uint32_t * arg);
    void (* function_pointer)(uint32_t * arg);
    uint32_t * var = (uint32_t *) some_ptr;
    ```

## Annotations

* Put `const` AFTER the object it annotates. The reason is that we read types
    from right to left:

    ```c
    /** mutable pointer to constant char */
    char const * a;
    /** constant pointer to mutable char */
    char *const a;
    /** constant pointer to constant char */
    char const *const a;
    ```

* Annotate in/out/inout arguments in the following way:
    - `_in_ Type Arg` = `Type const * Arg` (read-only value)
    - `_out_ Type Arg` = `Type /*out*/ * Arg` (write-only value)
    - `_inout_ Type Arg` = `Type * Arg` (both readable and writable)

* Annotate pointers in public interfaces.

    ```c
    /** x is a non-nullable pointer to one `char const` */
    void f(char const * x);

    /** x is a nullable pointer to one `char const` */
    void f(char const */*?*/ x);

    /** x is a non-nullable pointer to an array of `char const` */
    void f(char const */*[]*/ x);

    /** x is a nullable pointer to an array of `char const` */
    void f(char const */*[]?*/ x);
    ```

* Put pointer annotations together with the asterisk:

    ```c
    int * x;
    int */*[]?*/ x;
    int *const x;
    int */*[]?*/const x;
    int */*out*/ x;
    int */*[]? out*/ x;
    ```

* Annotate static functions with `static` on its own line:
    ```c
    static
    void f(...)
    {
        // ...
    }
    ```

* Annotate unused args with a preceding underscore and `FJ_ARG_UNSUED` macro:
    ```c
    void f(int _unused_arg)
    {
        FJ_ARG_UNSUED(unused_arg)
    }
    ```

* Annotate opaque pointer args with a following underscore and convert them with
    `FJ_ARG_FROM_OPAQUE` macro:
    ```c
    FJ_DEFINE_OPAQUE_TYPE(number_t)

    void f(number_t * number_)
    {
        FJ_ARG_FROM_OPAQUE(number, int *)
        *number = 123;
    }
    ```

## Naming

You can use the following common structure name suffixes:

| Suffix | Meaning | Usage | Description |
| ------ | ------- | ----- | ----------- |
| `_iface` | interface | library -> user | interface methods |
| `_callbacks` | callbacks | user -> library | interface callbacks |
| `_caps` | capabilities | library -> user | supported features for some interface |
| `_info` | information | user -> library | any information to create/update objects |
| `_desc` | description | library -> user | any information about objects that have been created/updated |

## Errors

Fallible function example:

```c
fj_err_t func(void)
{
    FJ_INIT_TRY

    initialise_something();

    FJ_TRY(do_something_fallible()) {
        terminate_something();
        return FJ_RESULT;
    }

    proceed_normally();

    return FJ_OK;
}
```

## More

Use Fejix base types, memory allocation and utils.

See:
* [fejix/core/base.h](../../include/fejix/core/base.h)
* [fejix/core/malloc.h](../../include/fejix/core/malloc.h)
* [fejix/core/utils.h](../../include/fejix/core/utils.h)

**TODO: Finish docs.**
