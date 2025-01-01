# Coding guidelines

Here are some consistency guidelines.

## Formatting

Use `.clang-format` and note a few things:

* Put trailing commas wherever possible.
* Separate separate function difinitions or any types of definition blocks with 2 empty lines.

## Documenting

* Use any doc comments (even empty ones, `/** */`) for any objects you want to appear in the docs.
* Wrap doc comments with `/** `..` */` or `/**`*\<newline\>*..*\<newline\>*`*/`
  and indent every line inside the comment:
    ```c
    /** One-line comment. */
    void foo(void);

    /**
        Multi-line comment.
        Multi-line comment.
    */
    void foo(void);
    ```
* Wrap file-level doc comments with `/**`*\<newline\>*..*\<newline\>*`*///`
  and indent every line inside the comment:
    ```c
    /**
        This file does foo.
        It also does bar.
    *///
    ```

## Naming

| Prefix | Meaning                   |
| ------ | ------------------------- |
| `out_` | Output function parameter |
| `on_`  | Event callback function.  |

| Suffix         | Meaning                                                              |
| -------------- | -------------------------------------------------------------------- |
| `_class`       | Class functions (provided by the library)                            |
| `_callbacks`   | Class callbacks (provided by the user)                               |
| `_manager`     | Class manager (global class data)                                    |
| `_info`        | Any information provided by the user or by the library to the user   |
| `_create_info` | Information to create some object                                    |

## Errors

Fallible function example:

```c
fj_err_t do_something_or_return_error(void)
{
    create_x();

    FJ_TRY (do_something_with_x_or_return_error()) {
        destroy_x();
        return fj_result; // defined implicitly by FJ_TRY
    }

    do_something_else_with_x();

    return FJ_OK;
}
```

## More

Use Fejix base types, memory allocation and utils.

See:
* [fejix/core/base.h](../../include/fejix/core/base.h)
* [fejix/core/alloc.h](../../include/fejix/core/alloc.h)
* [fejix/core/utils.h](../../include/fejix/core/utils.h)

**TODO: Finish docs.**
