#!/bin/sh

docker run --rm -it -v "$(pwd)/src":/project nlknguyen/alpine-mpich
