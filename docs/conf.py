# Sphinx configuration

import sys, os

sys.path.append(os.path.abspath('.')) # For importing local modules

project = 'Fejix'
author = 'Mark Lagodych'

_source_url = 'https://github.com/FejixGUI/fejix'
_creation_year = '2024'

from version import *

exclude_patterns = ['**/.*', '**/__*']
templates_path = ['_templates']

copyright = f'{_creation_year}-%Y {author}'
time_fmt = '%Y-%m-%d'
primary_domain = 'c'
highlight_language = 'C'
pygments_style = 'tango'
pygments_dark_style = 'dracula'

html_theme = 'furo'
html_title = project
html_static_path = ['_static']
html_last_updated_use_utc = True
html_last_updated_fmt = '%Y-%m-%d, %H:%M UTC'
html_show_sourcelink = True
html_copy_source = False
html_context = {
    'source_url': _source_url
}
html_theme_options = {
    'source_repository': _source_url,
    'source_branch': 'main',
    'source_directory': 'docs/',
    'top_of_page_buttons': ['edit'],
}

