===================
Development
===================


Documentation
=================

The project documentation is built using:

* `Doxygen <https://www.doxygen.nl/>`_
* `Sphinx <https://www.sphinx-doc.org/en/master/>`_
* `Breathe <https://breathe.readthedocs.io/en/latest/index.html>`_
* `Furo <https://pradyunsg.me/furo/>`_

Setup
------------

#. Install Doxygen.

#. `Create <https://docs.python.org/3/library/venv.html#creating-virtual-environments>`_
   and `activate <https://docs.python.org/3/library/venv.html#how-venvs-work>`_
   a Python virtual environment:

   .. code-block:: shell

     python3 -m venv .venv
     source .venv/bin/activate  # or activate.fish, or activate.csh

   The activation script path depends on the OS and the Python version.

Build
------------

Run ``Make`` from the ``docs`` directory:

.. code-block:: shell

  cd docs && make
  # -or- (shorter)
  make -C docs

To preview the docs in your browser you can run:

.. code-block:: shell

  make -C docs preview

this opens ``build/docs/html/index.html``.

Markup
------------

.. TODO give an overview of how the docs are structured

.. tip::
  Use the project's Doxygen command aliases:

  * ``\HEADER``

  instead of ``\file`` because it additionally generates a short include
  statement in the header description, and

  * ``\BEGIN{GROUP_NAME}``
  * ``\BEGIN{GROUP_NAME,GROUP_TITLE}``
  * ``\END``

  instead of repeating ``\addtogroup`` and ``\{`` on separate lines.
  To add a group description, just put a doc comment right above the ``\BEGIN``
  command.


.. tip::
  Write ``\---`` instead of ``---`` for mdashes in Doxygen comments
  (same applies for ndashes, ``\--``).
  As Doxygen output gets further processed by Sphinx, apparently XML entities
  in the intermediate output get escaped.
  This looks like this:

  .. list-table::

    * - Doc comment
      - Doxygen output (XML)
      - Sphinx output (HTML)
    * - ``---``
      - ``&8212;``
      - &8212;
    * - ``\---``
      - ``---``
      - ---


