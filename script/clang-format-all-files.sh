#!/bin/bash

# Go to root dir
cd "$(dirname "$0")"
cd .. 

# For each code file
echo ""
echo "Formatting files... ======"
for f in $(find include/* src/* -type f | grep -E "\.h$|\.tcc$|\.cpp$" | grep -v "ThirdParty")
do
	# Format file inplace
	clang-format --assume-filename=.clang-format -i $f
	echo "  Formatted '$f'" 
done

echo "=========================="
echo ""
