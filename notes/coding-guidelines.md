# Coding guidelines

Here are some consistency guidelines.

## Formatting

Use `.clang-format` with these few quirks:

* Put trailing commas wherever possible.
* Separate function difinitions with 2 empty lines.

## Documenting

* Use any doc comments (even empty ones, `/** */`) for any objects you want to appear in the docs.
* Wrap doc comments with `/** `..` */` or `/**`*\<newline\>*..*\<newline\>*`*/`
  with every intermediate line starting with a `*`:
    ```c
    /** One-line comment. */
    void foo(void);

    /**
     * Multi-line comment.
     * Multi-line comment.
     */
    void foo(void);
    ```
* Wrap file-level doc comments with `/**`*\<newline\>*..*\<newline\>*`*///`
  with every intermediate line starting with a `*`:
    ```c
    /**
     * This file does foo.
     * It also does bar.
     *///
    ```

## Naming

You can use the following common structure name suffixes:

| Suffix | Meaning |
| ------ | ------- |
| `_funcs` | Interface functions (provided by the library) |
| `_callbacks` | Interface callbacks (provided by the user) |
| `_manager` | Interface manager (global interface data) |
| `_info` | Any information provided by the user or by the library to the user |

## Errors

Fallible function example:

```c
fj_err_t func(void)
{
    initialise_something();

    FJ_TRY (do_something_fallible()) {
        terminate_something();
        return fj_result; // defined implicitly by FJ_TRY
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
