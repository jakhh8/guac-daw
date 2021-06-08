mkdir guac-daw/vendor/openal-soft/build
cd guac-daw/vendor/openal-soft/build
cmake ..
make
cd ../../../../
vendor/premake/bin/Linux/premake5 vs2019