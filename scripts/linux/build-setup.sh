# Builds the files and the necessities for the CommsProtocol library.
#!/bin/bash

path=$(dirname "$0")

echo ${path}

cd ${path}/../../

bit64="build64"
echo "Creating build directory"
mkdir build
cd build

if [ $1 = $bit64 ];
  then
    accept=ON
else
    accept=OFF
fi  

echo "Building files via cmake"
cmake -Dbuild64=${accept} ../

echo "Script Completed!!"
