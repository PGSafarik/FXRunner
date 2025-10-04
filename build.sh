#! /bin/sh

BD="build" 

if [ -e "$BD" ]; then
  echo "** Clear old build files"
  rm -rf "$BD/*"
else
  echo "** Create build directory"
  mkdir "$BD";  
fi

cd "$BD"; cmake .. && make && fxrunner; cd ..


