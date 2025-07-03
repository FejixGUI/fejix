import sys, os

sys.path.append(os.path.abspath("."))

from version import *

project = 'Fejix'
author = 'Mark Lagodych'
copyright = '2024-%Y ' + author
time_fmt = '%Y-%m-%d'

primary_domain = 'c'
highlight_language = 'C'

pygments_style = 'monokai'
html_theme = 'furo'

exclude_patterns = ['.*', '__*']

extensions = ['breathe']

breathe_projects = { project: '../build/docs/xml' }
breathe_default_project = project
breathe_domain_by_extension = { 'h': 'c' }
breathe_show_include = False
breathe_show_enumvalue_initializer = True
breathe_show_define_initializer = True
