#!/bin/sh

printHelp(){
  echo "Usage: ./build.sh [options]"
  echo "  --help              Prints help message."
  echo "  --proc              Number of processors for build."
  echo "  --unit-tests        Builds with unit tests."
  echo "  --run-unit-tests    Builds and runs unit tests."
}

parseArgs(){
  while [ $# -ge 1 ] ; do
    case "$1" in
      --help)
        printHelp
        exit 0
        ;;
      --proc)
        shift
        ;;
      --build_unit_tests)
        build_unit_tests=true
        shift
        ;;
      --run_unit_tests)
        run_unit_tests=true
        shift
        ;;
      *)
        echo "Invalid argument -- '$arg'"
        printHelp
        exit 1
        ;;
    esac
  done
}

mkdir -p .build

cd .build

# must run from project root directory - could be improved
CMAKE_ARGS="-DCMAKE_INSTALL_PREFIX=../build"

# initialize variables
build_unit_tests=false
run_unit_tests=false

parseArgs $@

if [ "$build_unit_tests" = true ] ; then
  CMAKE_ARGS="$CMAKE_ARGS -DBUILD_UNIT_TESTS=ON"
  echo "Building unit tests"
fi

if [ "$run_unit_tests" = true ] ; then
  CMAKE_ARGS="$CMAKE_ARGS -DBUILD_UNIT_TESTS=ON -DRUN_UNIT_TESTS=ON"
  echo "Building and running unit tests"
fi

if ! cmake $CMAKE_ARGS .. ; then
  echo "!!!!!!!!!! CMake failed !!!!!!!!!!"
  exit 1
fi

if ! make install -j$proc ; then
  echo "!!!!!!!!!! Build failed !!!!!!!!!!"
  exit 1
fi

cd ..