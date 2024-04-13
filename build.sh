#!/usr/bin/bash

mkdir -p /tmp/build
cd /tmp/build
cmake /workspaces/opengl
cmake --build .
