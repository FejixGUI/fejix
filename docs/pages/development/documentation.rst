=============
Documentation
=============

The project documentation is built using:

-   `Sphinx documentation generator <https://www.sphinx-doc.org/en/master/>`_
-   `Hawkmoth Sphinx extension for C <https://hawkmoth.readthedocs.io/en/stable/index.html>`_
-   `Furo theme <https://pradyunsg.me/furo/>`_

Build
================

Setup
-----------------

#.  `Create <https://docs.python.org/3/library/venv.html#creating-virtual-environments>`_
    and `activate <https://docs.python.org/3/library/venv.html#how-venvs-work>`_
    a Python virtual environment:

    ..  code-block:: shell

        python3 -m venv .venv --system-site-packages
        source .venv/bin/activate  # or activate.fish, or activate.csh

    The activation script path depends on the OS and the Python version.

#.  Follow the `Hawkmoth installation instructions <https://hawkmoth.readthedocs.io/en/stable/installation.html>`_
    for libclang and its Python bindings:

    ..  code-block:: shell

        sudo apt install python3-clang

    In case Hawkmoth fails to find libclang, you will likely need to set
    ``LD_LIBRARY_PATH`` as noted in the instructions:

    ..  code-block:: shell

        export LD_LIBRARY_PATH="/usr/lib/llvm-18/lib:$LD_LIBRARY_PATH"



Generate
-----------------

..  code-block:: shell

    cd docs && make && open build/index.html

Development mode
-----------------

This is faster, interactive, but unreliable --- when Sphinx builds
incrementally, the side menu often gets out of sync.

..  code-block:: shell

    cd docs && make dev


Markup
================

reStructuredText
------------------

Below is the demonstration of how reST truly works.
Always follow this style even despite the
`Sphinx documentation <https://www.sphinx-doc.org/en/master/usage/restructuredtext/basics.html#directives>`_
and
`Hawkmoth requirements <https://hawkmoth.readthedocs.io/en/stable/syntax.html#documentation-comments>`_
because this style is simple, consistent and seems to work perfectly fine with
the current versions of Sphinx and Hawkmoth.

..  code-block:: rst

    ..  note::

        Always use the consistent indentation style defined by the project.

    *   However reST indentation works somewhat differently.
    *   -   Markup starts with the outer indentation.
        -   But inner text must have inner indentation.

    ..  code-block:: c
        :emphasize-lines: 1

        // Notice how everything, even directive names and attributes,
        // is indented with 4 spaces.

In case you're wondering, this renders just normally (the result is
deliberately put inside a blockquote):

    ..  note::

        Always use the consistent indentation style defined by the project.

    *   However reST indentation works somewhat differently.
    *   -   Markup starts with the outer indentation.
        -   But inner text must have inner indentation.

    ..  code-block:: c
        :emphasize-lines: 1

        // Notice how everything, even directive names and attributes,
        // is indented with 4 spaces.

.. seriously though, reST indentation is somewhat weird

C comments
------------------

See the `Hawkmoth documentation <https://hawkmoth.readthedocs.io/en/stable/examples.html>`_.

Here is a little project-specific example:

..  code-block:: c

    /**
        .. _headerdoc:

        ===========
        myfile.h
        ===========

        This is the documentation for myfile.

        Here is the definition for :enum:`Fruit`:

        ..  autoenum:: Fruit
            :members:

        Here is the definition for :func:`sum`:

        ..  autofunction:: sum

        .. Comments that are standalone in code must be followed by a normal
        .. comment like that that '//' at the end.
    *///


    /**
        Note that EVERYTHING you want to see in the docs MUST be commented,
        empty comments are sufficient.
    */
    enum Fruit {
        /** */
        FRUIT_APPLE,
        /** */
        FRUIT_BANANA,
    };


    /**
        This finds the sum of two integers.
        :param a: The first integer.
        :param b: The second integer.
        :return: The sum of the two integers.
    */
    int sum(int a, int b);

Now this can be rendered in the docs with the following directive:

..  code-block:: rst

    ..  autosection:: headerdoc
        :file: myfile.h

The reason for manually referencing all the definitions in the header
documentation comment (headerdoc) is that this allows for structuring
the documentation in a way that makes sense, particularly it allows putting
different definitions under custom section headings.
