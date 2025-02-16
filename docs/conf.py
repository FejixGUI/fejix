import os

from hawkmoth.util import compiler
from clang.cindex import Config

project = 'Fejix'
copyright = '2024, Mark Lagodych'
author = 'Mark Lagodych'
release = '0.0.1'

extensions = ['hawkmoth', 'sphinx.ext.autodoc']

templates_path = ['_templates']
exclude_patterns = ['_build*', 'build*', '.*', 'Thumbs.db', '.DS_Store']

html_theme = 'furo'
html_static_path = ['_static']

script_dir = os.path.dirname(os.path.realpath(__file__))
hawkmoth_root = os.path.abspath(os.path.join(script_dir, "..", "src", "headers"))
print('[docs/conf.py] using root path:', hawkmoth_root)

if os.environ.get('DOCS_DEFAULT_CLANG_ARGS') is not None:
    hawkmoth_clang = os.environ.get('DOCS_DEFAULT_CLANG_ARGS').split(":")
    print('[docs/conf.py] using provided default clang args:', hawkmoth_clang)
else:
    hawkmoth_clang = compiler.get_include_args()
    print('[docs/conf.py] using default clang args:', hawkmoth_clang)

hawkmoth_clang += [
    '-std=c99',
    '-I' + os.path.join(hawkmoth_root),

    # We are not using Clang, we are using a Clang library, which does not have to understand
    # even definitions of built-in types! Suppress all warnings possibly arising from that.
    '-w',
]

if os.environ.get('DOCS_LIBCLANG_PATH') is not None:
    libclang_path = os.environ.get('DOCS_LIBCLANG_PATH')
    Config.set_library_file(libclang_path)
    print('[docs/conf.py] using libclang path:', libclang_path)
