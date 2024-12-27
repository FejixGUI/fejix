import os

from hawkmoth.util import compiler

project = 'Fejix'
copyright = '2024, Mark Lagodych'
author = 'Mark Lagodych'
release = '0.0.1'

extensions = ['hawkmoth']

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

html_theme = 'furo'
html_static_path = ['_static']

fejix_root = os.path.abspath('..')

hawkmoth_root = fejix_root
hawkmoth_clang = compiler.get_include_args()
hawkmoth_clang += ['-std=c99']
hawkmoth_clang += ['-I' + os.path.join(fejix_root, 'include')]
