#!/bin/bash

cd SoundApp/vendor/openal-soft/build
cmake ..
make
cd ../../../../
vendor/premake/bin/Linux/premake5 gmake2 --cc=gcc