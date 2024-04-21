#! /bin/sh

BD="build" 

if [ -e "$BD" ]; then
  rm -rf "$BD/*"
else
  mkdir "$BD";  
fi

cd "$BD"; cmake .. && make && fxrunner; cd ..


