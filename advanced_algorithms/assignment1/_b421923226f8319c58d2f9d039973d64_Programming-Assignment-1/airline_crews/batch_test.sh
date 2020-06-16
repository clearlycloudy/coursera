#!/bin/bash


files=$(find -iname '[0-9]?' | sort)
for i in $files
do
    time ./out < $i 1> /dev/null
done

