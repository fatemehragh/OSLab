#!/bin/bash

echo "File name:"
read file
echo "enter x"
read x
echo "enter y"
read y
echo ""
tail -n +$x $file | head -n $(($y-$x+1))
