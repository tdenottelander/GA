#!/bin/bash
path_to_executable=dist/Release/GNU-Linux/
path_to_project=/export/scratch1/tdo/TomGA/

evals=10000000
fit="ark8 14"
convergence=entire_pareto
optimizer=NSGA-II
repetitions=30
IMS=0

for popsize in 50 200 500 1000 #100
do
  for var in 2p #1p 2p 3p uni ark6
  do
    $path_to_executable/tomga -P $path_to_project -I $IMS -p $popsize -e $evals -f $fit -c $convergence -v $var -o $optimizer -r $repetitions
  done
done
