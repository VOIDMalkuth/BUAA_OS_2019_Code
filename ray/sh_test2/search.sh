#!bin/bash
#First you can use grep (-n) to find the number of lines of string.
#Then you can use awk to separate the answer.

awk "/$2/ {print NR}" $1 > $3
