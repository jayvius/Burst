#!/usr/bin/env sh

# No need for fancy Makefile; it's fast enough to just rebuild entire project every time

debug=false
if [ $# -eq 1 ] && [ $1 == "debug" ]; then
    debug=true
elif [ $# -eq 1 ]; then
    echo "ERROR: invalid argument $1"
    exit 1
fi

CPPFLAGS="-Wall -std=c++23 -Wno-deprecated"
OUTPUT=
if [ "$debug" = true ]; then
    echo "Building debug version..."
    CPPFLAGS="${CPPFLAGS} -g -D BURST_DEBUG"
    OUTPUT=burst_debug
else
    echo "Building release version..."
    CPPFLAGS="${CPPFLAGS} -O2"
    OUTPUT=burst
fi

g++ $CPPFLAGS \
    -I gl3w/include \
    -I /opt/homebrew/Cellar/glfw/3.4/include \
    -I /opt/homebrew/Cellar/glm/1.0.1/include \
    -L /opt/homebrew/Cellar/glfw/3.4/lib \
    -l glfw \
    -o $OUTPUT \
    src/burst.cpp \
    src/cube.cpp \
    src/buffer.cpp \
    src/vm.cpp \
    src/scanner.cpp \
    src/parser.cpp \
    gl3w/src/gl3w.c
