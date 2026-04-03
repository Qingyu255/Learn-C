#!/bin/sh
if [ -z "$1" ]; then
  echo "Usage: $0 <directory>"
  exit 1
fi

# why use &: only if gcc exits successfully, run ./$1/main
gcc "./$1/main.c" -o "./$1/main" && ./$1/main