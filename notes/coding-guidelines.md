# Coding guidelines

Here are some consistency guidelines.

## Formatting

Use `.clang-format` and note a few things:

* Put trailing commas wherever possible.
* Separate function difinitions or any definition blocks with 2 empty lines.

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

| Suffix         | Meaning            |
| -------------- | ------------------ |
| `_fn_t`        | Function typedef   |

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

**TODO: docs**


## More coding

Use Fejix base types, memory allocation and utils.

See:
* [fejix/core/base.h](../../include/fejix/core/base.h)
* [fejix/core/alloc.h](../../include/fejix/core/alloc.h)
* [fejix/core/utils.h](../../include/fejix/core/utils.h)

**TODO: docs**


## API design

### Return values

Because all interface functions must have a callable default implementation and all default
implementations are generated automatically, all functions must return one of the following:
* ``void`` (the default return value is not generated)
* ``bool`` (the default value is ``false``)
* ``fj_err_t`` (the default return value is ``FJ_ERR_UNIMPLEMENTED``)
* a pointer (must end with ``*``, typedefs are not supported) (the default value is ``NULL``)
* a numeric value (the default value is ``0``)

For any other return types the automatic code generator may fail to generate reasonable defaults
that compile reliably without warnings.

If a function needs to return a value of another type, it must do so with an output parameter
and return one of the supported return types.

Returning ``void`` in such case is sufficient if e.g. the returned object is retrieved
from an input object parameter, whose constructor function returns ``fj_err_t``. Example:
```c
/* If unimplemented, this returns FJ_ERR_UNIMPLEMENTED without touching the arguments. */
fj_err_t fj_xxx_create(struct xxx **out_xxx);

/* To call this, the user MUST make sure that xxx was created successfully. */
void fj_xxx_get_yyy(struct xxx *xxx, struct yyy **out_yyy);
```

As a side note, default implementations do not touch the output values, so the user must always
set the output parameters to NULL or other default values and always check for errors and/or if
the output value is unchanged.
It is even unnecessary for implementations to return NULLs to output parameters on failures.

**TODO: docs**
