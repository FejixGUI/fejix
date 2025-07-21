================================================================================
<fejix/base.h>
================================================================================

Defines base definitions and utilities.

Utilities
================================================================================

..  macro:: FJ_USERDATA(OBJECT)

    :expr:`(*((void **) (void *) (OBJECT)))`

    Gets the user data pointer from an object.
    The user data is always the first field of every object, so this macro
    legitimately just converts pointers.


Versions
================================================================================

..  type:: uint32_t fj_version

    A version number in the format of MAJOR.MINOR.PATCH, where each component
    is a 10-bit unsigned integer.
    The maximum value for each component is :math:`2^{10}-1 = 1023`,
    so the maximum version is ``1023.1023.1023``.
    This type is easily comparable: a smaller version number
    corresponds to an older version.

..  macro:: FJ_VERSION(MAJOR, MINOR, PATCH)

    :expr:`((MAJOR) << 20 | (MINOR) << 10 | (PATCH))`

    Constructs a version.

..  macro:: FJ_VERSION_MAJOR(VERSION)

    :expr:`(((VERSION) >> 20) & 0x3FF)`

    Gets the major component.

..  macro:: FJ_VERSION_MINOR(VERSION)

    :expr:`(((VERSION) >> 10) & 0x3FF)`

    Gets the minor component.

..  macro:: FJ_VERSION_PATCH(VERSION)

    :expr:`((VERSION) & 0x3FF)`

    Gets the patch component.

..  macro:: FJ_VERSION_NEXT_MAJOR(VERSION)

    :expr:`FJ_VERSION(FJ_VERSION_MAJOR(VERSION) + 1, 0, 0)`

..  macro:: FJ_VERSION_COMPATIBLE(ACTUAL, WANTED)

    :expr:`(((ACTUAL) >= (WANTED)) && ((ACTUAL) < FJ_VERSION_NEXT_MAJOR(WANTED)))`

    This checks if the ``ACTUAL`` version is compatible with the ``WANTED`` version using semantic versioning.

..  macro:: FJ_HEADER_VERSION

    :expr:`FJ_VERSION(current major, current minor, current patch)`

    This is the version of the header file itself, which may differ from the
    actually implemented version.

    The header version is used to check for API entries that the code can use,
    for example:

    ..  code-block:: c

        #if FJ_VERSION_COMPATIBLE(FJ_HEADER_VERSION, FJ_VERSION(1, 2, 3))
            // use some features from version 1.2.3
        #else
            // use some fallback code
        #endif

    To get the actually implemented version (which may differ per each platform
    and also depends on the actual dynamic library you are linking to),
    use :func:`fj_platform_get_version`.


Geometry
================================================================================

..  macro:: FJ_INCH_LENGTH

    :expr:`(0.0254)`

    The length of a metric inch in metres.

    This is used to convert between dots-per-metre (DPM) and dots-per-inch (DPI)
    and also for standard scaling factor calculations.

..  macro:: FJ_STANDARD_DPI

    :expr:`(96.0)`

    Concepts like text/interface scaling factor are mostly derived from the
    ratio to 96 DPI. That is, if the current DPI is 120, the content of the
    appropriate size is considered to be scaled by
    :math:`120 \div 96 = 125\%`
    compared to the "unscaled" ("density-unaware") content rendered at the
    standard 96 DPI.


..  enum:: fj_orientation

    A rectangular orientation, with flips and 90-degree rotations.

    ..  enumerator:: FJ_ORIENTATION_STANDARD

        The standard orientation implies that:

        -   (row 0, pixel 0) is at **top left**
        -   (row 0, pixel 1) is on the **right**
        -   (row 1, pixel 0) is at the **bottom**

    ..  enumerator:: FJ_ORIENTATION_ROTATED90

        Rotated 90 degrees clockwise.

    ..  enumerator:: FJ_ORIENTATION_ROTATED180

        Rotated 180 degrees clockwise.

    ..  enumerator:: FJ_ORIENTATION_ROTATED270

        Rotated 270 degrees clockwise.

    ..  enumerator:: FJ_ORIENTATION_FLIPPED_STANDARD

        Horizontally flipped (pixels in a row reversed).

    ..  enumerator:: FJ_ORIENTATION_FLIPPED_ROTATED90

        First horizontally flipped (pixels in a row reversed),
        then rotated 90 degrees clockwise.

    ..  enumerator:: FJ_ORIENTATION_FLIPPED_ROTATED180

        First horizontally flipped (pixels in a row reversed),
        then rotated 180 degrees clockwise.

    ..  enumerator:: FJ_ORIENTATION_FLIPPED_ROTATED270

        First horizontally flipped (pixels in a row reversed),
        then rotated 270 degrees clockwise.


..  type:: double fj_density

    Dots-per-metre (DPM).

    Convertible to dots-per-inch (DPI, see :macro:`FJ_INCH_LENGTH`)
    and standard scaling factor (see :macro:`FJ_STANDARD_DPI`).

..  struct:: fj_position2d

    Represents a position in 2D space.

    ..  member:: uint32_t x
    ..  member:: uint32_t y


..  struct:: fj_offset2d

    ..  member:: int32_t x
    ..  member:: int32_t y

..  struct:: fj_size2d

    ..  member:: uint32_t width
    ..  member:: uint32_t height

..  struct:: fj_rect2d

    Represents a real rectangular part of some visible area.

    ..  member:: fj_position2d position
    ..  member:: fj_size2d size

..  struct:: fj_viewport2d

    Represents a rectangle in coordinates relative to some visible area.

    ..  member:: fj_offset2d offset
    ..  member:: fj_size2d size

..  function:: static inline double fj_density_into_dpm(fj_density density)

..  function:: static inline fj_density fj_density_from_dpm(double dpm)

..  function:: static inline double fj_density_into_dpi(fj_density density)

..  function:: static inline fj_density fj_density_from_dpi(double dpi)

..  function:: static inline double fj_density_into_standard_scaling(fj_density density)

..  function:: static inline fj_density fj_density_from_standard_scaling(double scaling_factor)


Time
================================================================================

..  type:: uint64_t fj_time

    Time interval with nanosecond resolution in range from 1 nanosecond to 584
    years.

    This is used for time intervals, not absolute time, so it does not have a
    specific epoch. It is convertible to/from nanoseconds, microseconds,
    milliseconds and seconds.
..  function:: static inline fj_time fj_time_from_nanos(uint64_t nanoseconds)
..  function:: static inline uint64_t fj_time_into_nanos(fj_time time)
..  function:: static inline fj_time fj_time_from_micros(uint64_t microseconds)
..  function:: static inline uint64_t fj_time_into_micros(fj_time time)
..  function:: static inline fj_time fj_time_from_millis(uint64_t milliseconds)
..  function:: static inline uint64_t fj_time_into_millis(fj_time time)
..  function:: static inline fj_time fj_time_from_seconds(uint64_t seconds)
..  function:: static inline uint64_t fj_time_into_seconds(fj_time time)


Error handling
================================================================================

..  enum:: fj_err

    Generic error code.

    These error codes only indicate generic broad domains of problems that may
    have happened. To get a detailed error information, set
    :func:`fj_error_cb`.

    ..  enumerator:: FJ_OK

        Success.

    ..  enumerator:: FJ_ERR_MEMORY

        Out of memory.

    ..  enumerator:: FJ_ERR_SYSTEM

        System call failed.

        Indicates that a system operation like I/O (read/write/poll/etc.) has
        failed.

    ..  enumerator:: FJ_ERR_UNIMPLEMENTED

        Function unimplemented.

        Indicates that the operation is not implemented and therefore no work
        has been done.

    ..  enumerator:: FJ_ERR_UNSUPPORTED

        Unsupported feature.

        Indicates that the platform in general or its specific implementation
        does not support the feature.

    ..  enumerator:: FJ_ERR_INVALID

        Invalid operation.

        Indicates a programming error like zero allocation size, list index out
        of range, bad text encoding, removing from an empty list,
        a thread-unsafe operation done from another thread, using a result
        from an uncompleted task, canceling a completed task etc.

        This may often indicate a bug.

    ..  enumerator:: FJ_ERR_UNREACHABLE

        Host unreachable.

        Indicates that the server (e.g. the graphical system environment) does
        not respond.

    ..  enumerator:: FJ_ERR_REJECTED

        Request rejected.

        Indicates that the server has responded to the request with an error,
        often indicating that the request has been rejected,
        which may happen because of various reasons from a library bug to
        invalid library usage.

        When possible, the library provides an error message in such cases.


..  var:: void (*fj_error_cb)(char const *message)

    Called every time the library sets an error message.

    This is initialized to a default callback that does nothing for release
    builds and prints errors to stderr for debug builds.

    This function *must not* store the message pointer as the string may be
    freed immediately afterwards. The string must be copied in order to be
    stored.

    This should be thread-safe as some operations may be performed from other
    threads like :func:`fj_app_ping`.


Memory management
================================================================================

..  var:: void *(*fj_allocate_cb)(void *pointer, size_t old_size, size_t new_size)

    :param old_size:
        This can be 0 indicating that the memory must be newly allocated.

    :param new_size:
        This is never equal to ``old_size``.
        This can be 0 indicating that the memory must be freed.

    :returns: NULL on allocation failure or when freeing.

    Called every time the library needs to manage memory allocation.

    This can be set to a custom function that will be used for all memory
    allocations and deallocations done by the library.

    The default implementation is a no-op that does nothing and returns NULL,
    which means that the library will not allocate any memory.

    If you want to use the default implementation, set this to NULL.

    The callback must not be thread-safe as it is only called from the main
    thread.
