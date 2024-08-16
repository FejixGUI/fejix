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

## Errors

Fallible function example:

```c
fj_err_t some_func(void)
{
    FJ_WITH_ERRORS

    FJ_TRY(other_func()) {
        return FJ_RESULT;
    }

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
