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
pygments_style = 'monokai'
html_theme = 'furo'
extensions = ['breathe']

breathe_projects = { project: '../build/docs/xml' }
breathe_default_project = project
breathe_domain_by_extension = { 'h': 'c' }
breathe_show_include = False
breathe_show_enumvalue_initializer = True
breathe_show_define_initializer = True
breathe_default_members = ('members', 'undoc-members')

html_title = project
html_last_updated_use_utc = True
html_last_updated_fmt = '%Y-%m-%d, %H:%M UTC'
html_theme_options = {
    'source_repository': _source_url,
    'source_branch': 'main',
    'source_directory': 'docs/',
}
html_context = {
    'source_url': _source_url
}
