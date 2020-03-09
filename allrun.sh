#!/bin/bash
path_to_executable=dist/Release/GNU-Linux/

evals=10000000
fit="ark8 14"
convergence=entire_pareto
variation=2p
repetitions=30
IMS=0
popsize=100

# for optimizer in MO-RS MO-LS NSGA-II
# do
  # $path_to_executable/tomga -I $IMS -p $popsize -e $evals -f $fit -c $convergence -v $variation -o $optimizer -r $repetitions
# done

optimizer=MO-LS
evals=1000000
$path_to_executable/tomga -I $IMS -p $popsize -e $evals -f $fit -c $convergence -v $variation -o $optimizer -r $repetitions

optimizer=NSGA-II
evals=10000000
$path_to_executable/tomga -I $IMS -p $popsize -e $evals -f $fit -c $convergence -v $variation -o $optimizer -r $repetitions

IMS=1
optimizer=MO-GOMEA
for fos in uni learned
do
  # for popsize in 50 100 200 500 1000
  # do
    $path_to_executable/tomga -I $IMS -e $evals -f $fit -c $convergence -F $fos -o $optimizer -r $repetitions
  # done
done
