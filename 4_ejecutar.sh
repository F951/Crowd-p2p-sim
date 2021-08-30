#!/bin/bash

TOP=24
for (( j=0; j<=$TOP; j++ ))
do
	#./run1-$j.sh 1 &
	./run-param-$j.sh $1 &
done

echo "*** FINAL ***"
