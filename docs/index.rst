=============
Introduction
=============

.. toctree::
  :hidden:
  :maxdepth: 1

  self
  quick-start
  tutorials
  architecture
  features-and-plans
  build
  reference
  Source <https://github.com/FejixGUI/fejix>
  development
  changelog

.. TODO:
.. Change this wordy marketing text to an actually useful index page

.. TODO:
.. Probably move this to an "Architecture" page and change the wording to
.. "Goals" and "Non-Goals"

Fejix is a portable platform abstraction library written in pure C,
offering an industrial-level feature set.
It focuses on (but is not limited to) GUI, and is designed to power
UI frameworks, browsers, and game engines.

**Key Features:**

* **Low-Level Platform Access:**
  provides direct access to essential system resources and platform-specific
  features.

* **Cutting-Edge UI Capabilities:**
  supports modern platform features crucial for rich and responsive user
  interfaces, including (but not limited to):

  * System compositor integration

  * Transactional screen updates

  * Asynchronous operations

* **Full Range of UI Features**:
  window management, input handling, system notifications, clipboard,
  notifier icons, etc.
  I would be happy to support as much useful features as reasonably possible.

* **Modular Codebase:**
  highly configurable, allowing compilation into anything from a minimal
  library (like GLFW or SDL) to an advanced base (like Qt or a browser engine)
  simply by adjusting build options.

* **Extensible and Portable:**
  designed for easy extension, porting, or even reimplementation.
  This is achieved because:

  * Platforms are (almost) independent chunks of code.

  * Features can be implemented independently, in any amount and order.

  * There is no single, monolithic wrapper abstraction around the platforms.
    Platforms directly expose the public API.

* **Compatibility with existing software**:
  Fejix implements some legacy or not-so-cross-platform features to allow
  existing projects (think SDL or Qt) to potencially migrate to itself without
  breaking their code (too much).

**Out of scope:**

* **Common utilities:**
  Fejix does not provide a batteries-included set of common utilities such as
  multitasking, synchronization primitives, internationalization resource
  loading, debugging/logging, file I/O, text encoding or basic container types.

* **Graphics rendering or widget handling:**
  these functionalities are expected to be implemented by other frameworks
  built on top of Fejix.

* **Multi-threaded API:**
  the API is currently designed for use only from the main thread.
  Due to varying platform limitations on thread safety and multi-threaded event
  processing support, the most reliable option is to keep the library
  single-threaded, requiring users to handle multitasking and implement
  communication with the main thread on their own.
