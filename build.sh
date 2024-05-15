#!/bin/bash

OUT_FILE="fashion-fashionating"

pushd data > /dev/null

# Function to extract filename without extension and clean it
get_clean_filename() {
    local fullpath="$1"
    local filename=$(basename -- "$fullpath")
    filename="${filename%.*}" # Remove extension
    filename="${filename//./_}" # Replace dots
    filename="${filename//-/_}" # Replace dashes
    filename="${filename// /_}" # Replace spaces
    echo "$filename"
}

# Music files
for mod in ../music/*.mod; do
    base_name=$(get_clean_filename "$mod")
    bin2h -i "$mod" -o "$base_name"
done

# PNG files
for png in ../graphics/*.png; do
    base_name=$(get_clean_filename "$png")
    png2h -i "$png" -o "$base_name"
done

bin2h -i ../music/fashion-sample.raw -o fashion-sample

popd > /dev/null

# Build Linux version
gcc -O3 -shared -fPIC -o "remake_$OUT_FILE.so" remake.c -I../../include
# gcc -g -O0 -shared -fPIC -o "remake_$OUT_FILE.so" remake.c -I../../include

# Build Windows version
x86_64-w64-mingw32-gcc -O3 -shared -o "remake_$OUT_FILE.dll" remake.c -I../../include

[ -e "remake_$OUT_FILE.so" ] && mv "remake_$OUT_FILE.so" ../../bin/remakes
[ -e "remake_$OUT_FILE.dll" ] && mv "remake_$OUT_FILE.dll" ../../bin/remakes

