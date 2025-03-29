================
Building docs
================


Preparation
=============

Install `Doxygen <https://www.doxygen.nl/download.html>`_ and
`Python <https://www.python.org/downloads/>`_.

Setup a Python environment
----------------------------

.. code-block:: console

    python3 -m venv .venv
    # Windows: py -m venv .venv

    . .venv/bin/activate
    # fish: . .venv/bin/activate.fish
    # csh: . .venv/bin/activate.csh
    # Windows console: .venv/Scripts/activate
    # Windows PowerShell: . .venv/Scripts/Activate.ps1

Install Python packages
----------------------------

.. code-block:: console

    pip install sphinx breathe furo


Build
===========

These commands must be run inside the docs directory.

.. code-block:: console

	doxygen doxygen.conf
	sphinx-build -M html . build
