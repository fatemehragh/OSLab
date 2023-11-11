#!/bin/bash

while(true)
do
echo "Enter your number:"
read x
out=0
sum=0
while [ $x -gt 0 ]
do
    digit=$(expr $x % 10)
    out=$(expr $out \* 10 + $digit)
    x=$(expr $x / 10)
    sum=$(expr $sum + $digit)
done
echo ""
echo "Reverse: $out"
echo "Sum of digits: $sum"
echo ""
done
