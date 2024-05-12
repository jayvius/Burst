#!/usr/bin/env sh

debug=false
if [ $# -eq 1 ] && [ $1 == "debug" ]; then
    debug=true
elif [ $# -eq 1 ]; then
    echo "ERROR: invalid argument $1"
    exit 1
fi

CPPFLAGS="-Wall -std=c++20 -Wno-deprecated"
if [ "$debug" = true ]; then
    echo "Building debug version..."
    CPPFLAGS="${CPPFLAGS} -g"
else
    echo "Building release version..."
    CPPFLAGS="${CPPFLAGS} -O2"
fi

g++ $CPPFLAGS \
    -I gl3w/include \
    -I /opt/homebrew/Cellar/glfw/3.4/include \
    -I /opt/homebrew/Cellar/glm/1.0.1/include \
    -I /opt/homebrew/Cellar/fmt/10.2.1_1/include \
    -L /opt/homebrew/Cellar/glfw/3.4/lib \
    -L /opt/homebrew/Cellar/fmt/10.2.1_1/lib \
    -l glfw \
    -l fmt \
    -o burst \
    src/burst.cpp \
    src/cube.cpp \
    src/buffer.cpp \
    src/vm.cpp \
    src/scanner.cpp \
    src/parser.cpp \
    gl3w/src/gl3w.c
