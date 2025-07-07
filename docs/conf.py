# Sphinx configuration

project = 'Fejix'
author = 'Mark Lagodych'

_source_url = 'https://github.com/FejixGUI/fejix'
_creation_year = '2024'

import sys, os
sys.path.append(os.path.abspath('.'))
from version import *

exclude_patterns = ['.*', '__*']
templates_path = ['_templates']

copyright = f'{_creation_year}-%Y {author}'
time_fmt = '%Y-%m-%d'
primary_domain = 'c'
highlight_language = 'C'
pygments_style = 'tango'
pygments_dark_style = 'dracula'
html_theme = 'furo'
extensions = ['breathe']

breathe_projects = { project: '../build/docs/xml' }
breathe_default_project = project
breathe_domain_by_extension = { 'h': 'c' }
breathe_show_include = False
breathe_show_enumvalue_initializer = True
breathe_show_define_initializer = True
breathe_default_members = ('members', 'undoc-members')
breathe_order_parameters_first = True

html_title = project
html_static_path = ['_static']
html_last_updated_use_utc = True
html_last_updated_fmt = '%Y-%m-%d, %H:%M UTC'
html_theme_options = {
    'top_of_page_buttons': []
}
html_context = {
    'source_url': _source_url
}
