# Fejix documentation

## Build

### Setup
```sh
pip install -r ./docs/requirements.txt
```

See [Hawkmoth installation guide](https://hawkmoth.readthedocs.io/en/stable/installation.html#clang-distro-install) for possible additional steps.

### Run
```sh
sphinx-build -M html ./docs ./build/docs
```

### Troubleshoot

If the libclang was not found or any compile errors occurred, try:

```sh
# Find out /path/to/libclang
where '*libclang*'

# Find out /path/to/standard/include1, /path/to/standard/include2, ...
clang -E - -v < /dev/null

DOCS_LIBCLANG_PATH="path/to/libclang" \
DOCS_DEFAULT_CLANG_ARGS="-I/path/to/standard/include1:-I/path/to/standard/include2:..." \
sphinx-build -M html ./docs ./build/docs
```
