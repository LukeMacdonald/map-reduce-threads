#!/bin/sh
grep -v '[^[:alpha:]]' $1 > temp.txt
sort temp.txt > $2
uniq -u temp.txt > $2
sed -r '/^.{3,}/!d' $2 > temp.txt
sed '/.\{15\}/d' temp.txt > $2
rm temp.txt
