# Coding style

Here are some consistency guidelines.

## Standard

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
    uint32_t * function(uint32_t * arg);
    void function(uint32_t * arg);
    void (* function_pointer)(uint32_t * arg)
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

    struct some_struct_type long_function_definition(
        struct argument_type const ** const *** some_long_argument,
        const struct argument_type ** const * another_argument
    )
    {

    }
    ```

* Put each variable declaration on its own line.

    ```c
    struct somestruct * a;
    struct somestruct * b;
    uint32_t i;
    uint32_t j;
    uint32_t k;
    ```

## Types

* Use `fj_bool_t` instead of `bool`. Use `true`/`false` instead of `1`/`0`.

    `fj_bool_t` has a fixed size, so it is more preferable in public
    declarations.

    We use C99, so `<stdbool.h>` and `true`/`false` are available.

* Use `fj_str_t` instead of `const char *`.

## Error handling

Use the Fejix macros for error handling.

See [fejix/utils.h](../../include/fejix/utils.h).

<!-- TODO Finish style notes -->
