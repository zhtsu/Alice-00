@echo off
cd ..
if not exist build mkdir build
cd build
if exist CMakeCache.txt del CMakeCache.txt
cmake -G "MinGW Makefiles" ..
mingw32-make -f Makefile
cd bin
Alice
pause
