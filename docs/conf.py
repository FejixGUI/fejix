project = 'Fejix'
copyright = '2025, Mark Lagodych'
author = 'Mark Lagodych'
release = '0.1.0'

exclude_patterns = ['README.rst', '.*', 'build/']

html_theme = 'furo'
html_static_path = ['static']
html_show_sourcelink = False
html_copy_source = False

pygments_style = 'monokai'
pygments_dark_style = pygments_style

extensions = ['breathe']

breathe_projects = { 'fejix': 'build/xml' }
breathe_default_project = 'fejix'
breathe_show_include = False
breathe_order_parameters_first = False
