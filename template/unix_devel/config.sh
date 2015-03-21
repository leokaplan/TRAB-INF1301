#!/bin/sh

cp -rf ./headers ../
mkdir -p ../bin
mkdir -p ../obj
mkdir -p ../lib
cd ../../arcaboucounix/arcabouc/Fontes/
make
cd -
cp -f ../../arcaboucounix/arcabouc/Fontes/libArcaboucoTeste.a ../lib/
