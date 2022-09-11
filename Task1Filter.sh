#!/bin/bash

if [ ! $# -eq 2 ] 
then 
    echo "Missing file, correct command is: 'Task1Filter.sh InputFileName OutputFileName'"
    exit 0
fi
if [ -e $1 ]
then
    grep -v '[^[:alpha:]]' $1 | sort | uniq -u | sed -r '/^.{3,}/!d' | sed '/.\{15\}/d' | sort -k 1.3  > $2
else
    echo "No input file provided! Try Again"
fi