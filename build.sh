#!/usr/bin/env bash

BUILD_DIR="./build"

mkdir -p ${BUILD_DIR} &&
  echo -e "Changing directory to ${BUILD_DIR}" &&
  cd build && echo -e ${pwd} && cmake .. && make && cd ../ && make run
