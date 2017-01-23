#!/bin/bash
./ant $(for i in $(seq 1 255); do echo $(($RANDOM % 2)); done | tr -d '\n') 500; gpicview test.pgm
