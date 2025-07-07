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

See the ``Makefile`` in the ``docs`` folder.

.. TODO Discuss the setup process and the documentation style/structure.

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


