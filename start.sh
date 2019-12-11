#!/bin/sh

sudo docker run --rm -it -v "$(pwd)/src":/project nlknguyen/alpine-mpich
