#!/bin/bash

# Build engine
cd engine/src
qmake
make

# Build qt-gui
cd ../../qt-gui/src
qmake
make

# Build doc
cd ../../doc
doxygen

echo "Done"
