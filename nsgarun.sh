#!/bin/bash
path_to_executable=dist/Release/GNU-Linux/

evals=10000000
fit="ark7 14"
convergence=entire_pareto
optimizer=NSGA-II
repetitions=30
IMS=0

for popsize in 50 100 200 500 1000
do
  for var in 1p 2p 3p uni ark6
  do
    $path_to_executable/tomga -I $IMS -p $popsize -e $evals -f $fit -c $convergence -v $var -o $optimizer -r $repetitions
  done
done
