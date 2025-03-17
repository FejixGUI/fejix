#!/usr/bin/env bash

python3 tools/generate_protocol_headers.py spec/*.json -d include/fejix/protocols

clang-format --style="file" -i include/fejix/protocols/*.h