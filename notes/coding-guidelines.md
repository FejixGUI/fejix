# Coding guidelines

Here are some consistency guidelines.

## Formatting

Use `.clang-format` with these few quirks:

* Put trailing commas wherever possible.
* Separate function difinitions with 2 empty lines.
* Use any doc comments (even empty ones, `/** */`) for any objects you want to appear in the docs.
* Embrace doc comments with `/** `..` */` or `/**`*\<newline\>*..*\<newline\>*`*/` with no additional formatting:
    ```c
    /** One-line comment. */
    void foo(void);

    /**
    Multi-line comment.
    Multi-line comment.
    */
    void foo(void);
    ```
* Embrace file-level doc comments with `/**`*\<newline\>*..*\<newline\>*`*///` with no additional formatting:
    ```c
    /**
    Multi-line comment.
    Multi-line comment.
    *///
    void foo(void);
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
