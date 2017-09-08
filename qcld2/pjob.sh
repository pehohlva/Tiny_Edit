#!/bin/bash
pwd
rm -rf build Makefile libqcld2.a
echo clean dir now
qmake && make && make install