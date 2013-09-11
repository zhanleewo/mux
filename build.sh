#! /bin/sh
mkdir -p build/
cd build/
rm -rf *
cmake ../src -G Xcode
open mux.xcodeproj
