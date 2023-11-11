#!/bin/bash

echo "Enter the first desired number :"
read a
if [[ -n ${a//[0-9]/} ]]; then
    echo "The given input is not a number!"
    exit 1
fi
echo "Enter the second desired number: "
read b
if [[ -n ${b//[0-9]/} ]]; then
    echo "The given input is not a number!"
    exit 1
fi
let sum=a+b
echo ""
echo "$a + $b = $sum" 
if [[ $a -gt $b ]]; then
    echo "$a is greater than $b"
elif [[ $a -lt $b ]]; then
    echo "$a is less than $b"
else
    echo "$a is equal to $b"
fi
