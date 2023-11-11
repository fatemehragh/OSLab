#!/bin/bash

while(true)
do
echo ""
echo "first number:"
read a

echo "second number:"
read b

echo "operator (+  -  /  x  e(Exit)):"
read op
echo ""
case $op in
    +) 
        let sum=a+b
        echo "$a + $b = $sum"
        ;;
    -) 
        let sum=a-b
        echo "$a - $b = $sum"
        ;;
    /) 
        let sum=a/b
        echo "$a / $b = $sum"
        ;;
    x) 
        let sum=a*b
        echo "$a * $b = $sum"
        ;;
    e) 
        exit 1
        ;;
    *) 
        echo "invalid operator!"
        ;;
esac
done
