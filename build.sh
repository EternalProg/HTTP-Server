#!/bin/bash
set -euo pipefail

# Default arg = debug
ARG="${1:-debug}"
ARG=${ARG,,} # lowercase

# --- Configs ---
declare -A DEBUG_CONFIG=(
  [MODE]="Debug"
  [DIR]="debug"
)

declare -A RELEASE_CONFIG=(
  [MODE]="Release"
  [DIR]="release"
)

# --- Helpers ---
usage() {
  echo "Usage: $0 [debug|release|run|test]"
  echo "  debug   : build Debug (default)"
  echo "  release : build Release"
  echo "  run     : build Debug and run executable"
  echo "  test    : build Debug and run tests"
  exit 1
}

info() {
  echo -e "\033[1;32m[INFO]\033[0m $*"
}

error() {
  echo -e "\033[1;31m[ERROR]\033[0m $*" >&2
}

# --- Build function ---
build() {
  local -n config=$1 # associative array reference

  info "Building in ${config[MODE]} mode"
  cmake -S . -B "build/${config[DIR]}" -DCMAKE_BUILD_TYPE="${config[MODE]}"
  cmake --build "build/${config[DIR]}"
}

# --- Main logic ---
case "$ARG" in
  debug|d)
    build DEBUG_CONFIG
    ;;
  release|r)
    build RELEASE_CONFIG
    ;;
  run)
    build DEBUG_CONFIG
    ./build/${DEBUG_CONFIG[DIR]}/HTTP_SERVER
    ;;
  test)
    build DEBUG_CONFIG
    info "Running tests"
    ctest --test-dir build/debug/tests
    ;;
  *)
    error "Unknown build mode: '$ARG'"
    usage
    ;;
esac
