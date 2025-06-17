#! /bin/bash
clear

echo 'Build libctrpf? (y/n):'

read buildLib

if ["$buildLib" = 'y']; then
	cd libctrpf/Library
	make -j 8
	cd ../..
else
	echo Skip libctrpf...
fi

make -j 8
echo finished
