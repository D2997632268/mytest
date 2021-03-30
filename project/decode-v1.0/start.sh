#!/bin/bash
source  /sunlux/work/junlin/x2000-source-code/source.sh 
rm -rf Makefile 
cp Makefile_junlin Makefile

rm -rf bin/*
make clean &&clear && make 
chmod 777 bin/decode
#./bin/code
 sz  $1 > /dev/ttyACM0  < /dev/ttyACM0
# sz  1.nv12  > /dev/ttyACM0  < /dev/ttyACM0 
