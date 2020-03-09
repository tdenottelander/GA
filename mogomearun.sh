#!/bin/bash
path_to_executable=dist/Release/GNU-Linux/

evals=10000000
fit="ark7 14"
convergence=entire_pareto
variation=2p
repetitions=30
IMS=0
optimizer="MO-GOMEA"

for popsize in 50 100 200 500 1000
do
  for fos in uni learned
	do
	$path_to_executable/tomga -I $IMS -p $popsize -e $evals -f $fit -c $convergence -F $fos -o $optimizer -r $repetitions
	done
done
