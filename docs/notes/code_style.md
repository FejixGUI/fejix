# Coding style

Here are some consistency guidelines.

## Standard

* Use C99.

## Comments

Use block comments without any additional decoration.
Use the following almost-plain-text syntax:
```c
/* Here is a summary of the function.

    Here is some additional information.

    === ARGS ===

    Here is something about the arguments.

    === RETURNS ===

    Here is something about the return value.

    === DETAILS ===

    Here is some more detailed information.
    Typically, this section is quite long.
    This may include quite a lot of text. */
void a(void);

```

Break paragraphs and multi-line list items with empty lines:

```
This is paragraph 1.
And this is too.

This is paragraph 2.
And so is this.

Shopping list:

- item 1
- item 2

Long shopping list:

- some very
    very long item 1

- some very
    very long item 2
```

You can use some formatting:

```
`inline code`

_emphasis_


- unordered
- list


1. ordered
2. list


- This item spans
    multiple lines.

- Please, put empty lines between
    such items.


- This is an item of an unordered list.

    + this is an item of a nested unordered list.

        ++ this item is nested even further

```

## General formatting

* Put `{` on the same line as the definition, except for functions.

* Put each variable declaration on its own line.

* Put spaces around `*`:
    ```c
    uint32_t * var;
    uint32_t * * var2;
    uint32_t * * * var3;
    uint32_t * function(uint32_t * arg);
    void function(uint32_t * arg);
    void (* function_pointer)(uint32_t * arg)
    ```

* Put `const` AFTER the object it annotates:

    ```c
    /* mutable pointer to constant char */
    char const * a;
    /* constant pointer to mutable char */
    char * const a;
    /* constant pointer to constant char */
    char const * const a;
    ```

## More

Use Fejix base types, memmory allocation and error handling.

See:
* [fejix/base.h](../../include/fejix/base.h)
* [fejix/malloc.h](../../include/fejix/malloc.h)
* [fejix/utils.h](../../include/fejix/utils.h)

<!-- TODO Finish style notes -->
