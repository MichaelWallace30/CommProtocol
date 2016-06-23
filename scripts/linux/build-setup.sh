# Builds the files and the necessities for the CommsProtocol library.
#!/bin/bash

path=${PWD/}

echo ${path}

cd ${path}/../../

bit64="build64"
echo "Creating build directory"
mkdir build
cd build

echo "$bit64"
if [ $1 = $bit64 ];
  then
    accept=ON
else
    accept=OFF
fi

echo ${accept}  

echo "Building files via cmake"
cmake -Dbuild64=${accept} ../
