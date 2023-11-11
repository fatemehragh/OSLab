#!/bin/bash

# Get the count of operands
read -p "Enter the count of operands: " operand_count

# Get the operands
for ((i=1; i<=$operand_count; i++)); do
    read -p "Enter operand $i: " operand
    operands+=($operand)
done

# Get the count of operators
read -p "Enter the count of operators: " operator_count

# Get the operators
for ((i=1; i<=$operator_count; i++)); do
    read -p "Enter operator $i (+, -, x, /): " operator
    operators+=($operator)
done

# Perform the calculations with proper precedence
for ((i=0; i<$operator_count; i++)); do
    case ${operators[i]} in
        x) operands[i+1]=$((${operands[i]} * ${operands[i+1]}))
           let operands[i]=operands[i+1];;
           
        /) operands[i+1]=$((${operands[i]} / ${operands[i+1]}))
           let operands[i]=operands[i+1];;
    esac
done

result=${operands[0]}
for ((i=0; i<$operator_count; i++)); do
    case ${operators[i]} in
        +) result=$((result + ${operands[i+1]}));;
        -) result=$((result - ${operands[i+1]}));;
    esac
done

# Display the result
echo "Result: $result"
