#!/bin/bash

echo "" > $2

for i in {8,32,128,512,1024} 
do
	sed -n -e "$i p" $1 >> $2;
done
