#!/bin/bash
#variable assignment
# no space around = during assignment
a=24
echo $a
echo "$a"
echo "The value of\"a\" is $a."
a=`echo Hello!`
echo $a
a=`ls -l`
echo "$a"
echo $a
let a=16+5
echo " the value is now $a."
