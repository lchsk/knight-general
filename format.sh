#!/usr/bin/env bash

clang-format -i `find src -type f | xargs`
