=================
Code style
=================



TODO, FIXME, XXX
----------------------------

Mark the important things with the following set of markers:

*   For a work in progress use ``TODO``:

    In ordinary C comments:

    ..  code-block:: c

        // TODO <message>

    In RST documentation:

    .. code-block:: rst

        .. TODO <message>

* For bugs that need to be fixed use ``FIXME`` near the code they appear:

    In C comments:

    ..    code-block:: c

        // FIXME <message>

    In RST documentation:

    ..    code-block:: rst

        .. FIXME <message>

* For important implementation notes use ``XXX`` near the code that relies on
  or works around some badly documented or surprising platform features:

    ..  code-block:: c

        // XXX <message>

This makes it easy to search for places in code that require attention with
``ripgrep``:

..  code-block:: shell

    rg 'TODO|FIXME|XXX'
