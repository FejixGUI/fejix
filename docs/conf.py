# Sphinx configuration

import sys, os

sys.path.append(os.path.abspath('.')) # For importing local modules

project = 'Fejix'
author = 'Mark Lagodych'

_github_url = 'https://github.com/FejixGUI/fejix'
_creation_year = '2024'
_include_path = os.path.abspath('../include')

from version import *

exclude_patterns = ['**/.*', '**/__*']
templates_path = ['_templates']

copyright = f'{_creation_year}-%Y {author}'
time_fmt = '%Y-%m-%d'
primary_domain = 'c'
highlight_language = 'C'
pygments_style = 'tango'
pygments_dark_style = 'dracula'
extensions = []

html_theme = 'furo'
html_title = project
html_static_path = ['_static']
html_last_updated_use_utc = True
html_last_updated_fmt = '%Y-%m-%d, %H:%M UTC'
html_show_sourcelink = True
html_copy_source = False
html_context = {
    # The link to the repo in the custom footer
    'source_url': _github_url
}

extensions += ['hawkmoth']
hawkmoth_root = _include_path
hawkmoth_clang = [f'-I{_include_path}', '-DDOCS']
# The links to header source code
hawkmoth_source_uri = f'{_github_url}/blob/main/include/{{source}}#L{{line}}'
