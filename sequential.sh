#!/usr/bin/env bash

for i in $(seq 2 127); do 
	ant=$(python -c "print(bin($i))" | cut -d 'b' -f2); 
	./ant $ant && cp test.pgm $ant.pgm;
done


