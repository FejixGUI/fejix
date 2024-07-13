# Coding style

Here are some consistency guidelines.

## General

* Use C99.

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

* Put each variable declaration on its own line:

    ```c
    // Do not merge these definitions into one line!
    uint32_t a;
    uint32_t b;
    ```

* Put spaces around `*`:
    ```c
    uint32_t * var;
    uint32_t * * var;
    uint32_t * * * var;
    uint32_t * const * var;
    uint32_t * * const var;
    uint32_t const * * const * var;
    uint32_t * function(uint32_t * arg);
    void function(uint32_t * arg);
    void (* function_pointer)(uint32_t * arg)
    ```

## Annotations

* Put `const` AFTER the object it annotates. The reason is that we read types
    from right to left:

    ```c
    /** mutable pointer to constant char */
    char const * a;
    /** constant pointer to mutable char */
    char * const a;
    /** constant pointer to constant char */
    char const * const a;
    ```

* Annotate in/out/inout arguments in the following way:
    - `_in_ Type Arg` = `Type const * Arg` (read-only value)
    - `_out_ Type Arg` = `Type /*out*/ * Arg` (write-only value)
    - `_inout_ Type Arg` = `Type * Arg` (both readable and writable)

* Annotate pointers in public interfaces.

    ```c

    /** x is a non-nullable pointer to one `uint8_t const` */
    void f(uint8_t const * x);

    /** x is a non-nullable pointer to an array of `uint8_t const` */
    void f(uint8_t const */*[]*/ x);

    /** x is a nullable pointer to one `uint8_t const` */
    void f(uint8_t const */*?*/ x);

    /** x is a nullable pointer to an array of `uint8_t const` */
    void f(uint8_t const */*[]?*/ x);

    ```

* Annotate static functions with `static` on its own line:
    ```c
    static
    void f(...)
    {
        // ...
    }
    ```

* Annotate extendable and extended structures (usually used for callbacks):

    ```c

    /*extendable*/
    struct callback {
        void (*call)(struct callback * this, int x);
    };

    void call_callback(struct callback * callback)
    {
        callback->call(callback, 123);
    }

    struct my_callback {
        struct callback /*extend*/ callback;
        int y;
    };

    void my_call(struct callback * _this, int x)
    {
        struct my_callback * this = (void *) _this;
        printf("%d + %d = %d\n", x, this->y, x + this->y);
    }

    int main(void)
    {
        struct my_callback my_callback = { { my_call }, 456 };

        call_callback((void *) &my_callback);
    }

    ```

## More

Use Fejix base types, memory allocation and error handling.

See:
* [fejix/base.h](../../include/fejix/base.h)
* [fejix/malloc.h](../../include/fejix/malloc.h)
* [fejix/error.h](../../include/fejix/error.h)

**TODO: Finish docs.**
