General TODO:
- [ ] Add a "Progress" page with a giant feature support matrix
    (vertical features, horizontal platforms).
- [ ] Add "Changelog" page
- [ ] Add a "How to" page: a relation "what I want to do" - "how to do that"
- [ ] Add examples
- [ ] Separate "Developer notes" from "Contribute"

Let's be honest, Doxygen is abysmal.
Doxygen TODO:
- [ ] Better search box (probably use Google or DuckDuckGo).
    This requires manual changes to `header.html`.
- [ ] Better page navigation on mobile.

    On mobile the menu looks like this:

    - Main page
    - Related pages

    It would probably be better if the user did not have to do an extra click:
    - Intro
    - Build
    - Tutorial
    - etc.

    How to do that in `layout.xml`:
    ```xml
    <tab type="user" visible="yes" url="@ref some-page" title=""/>
    ```
- [ ] Structs are missing from the API index.

    This is a fundamental behavior of Doxygen.
    Structs and other globals (e.g. typedefs, functions, enums) cannot be put in the same
    group automatically.
    We either manually specify various `\ingroup`s near every API entry or use something different.
- [ ] Switch to Doxygen/XML + Sphinx/Breathe or maybe Sphinx/c-autodoc or Sphinx/Hawkmoth
    (which requires a lot of ReST, quite some boilerplate from what I can tell and is overall
    a challenge on its own).
