=============
Documentation
=============

The project documentation is built using
`Sphinx documentation generator <https://www.sphinx-doc.org/en/master/>`_
and
`Furo theme <https://pradyunsg.me/furo/>`_.

Setup
================

#.  `Create <https://docs.python.org/3/library/venv.html#creating-virtual-environments>`_
    and `activate <https://docs.python.org/3/library/venv.html#how-venvs-work>`_
    a Python virtual environment:

    .. code-block:: shell

        python3 -m venv .venv
        source .venv/bin/activate  # or activate.fish, or activate.csh

    The activation script path depends on the OS and the Python version.

Build
================

..  code-block:: shell

    cd docs && make && open build/index.html

Development mode
-----------------

This is faster, interactive, but unreliable --- when Sphinx builds
incrementally, the side menu often gets out of sync.

..  code-block:: shell

    cd docs && make autobuild


Markup
================

A personal note to myself on indentation in reST:

..   code-block:: rst

    ..  note::

        Always use the consistent indentation style defined by the project.

    *   However reST indentation works somewhat differently.
    *   -   Markup starts with the outer indentation.
        -   But inner text must have inner indentation.

    ..  code-block:: c
        :emphasize-lines: 1

        // Notice how everything, even directive names and attributes,
        // is indented with 4 spaces.

In case you're wondering, this renders just normally:

..  note::

    Always use the consistent indentation style defined by the project.

*   However reST indentation works somewhat differently.
*   -   Markup starts with the outer indentation.
    -   But inner text must have inner indentation.

..  code-block:: c
    :emphasize-lines: 1

    // Notice how everything, even directive names and attributes,
    // is indented with 4 spaces.
