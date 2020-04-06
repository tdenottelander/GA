#!/bin/bash
path_to_executable=dist/Release/GNU-Linux/tomga
path_to_project=src/../

evals=10000000
fit="ark7 14 2"
# convergence=entire_pareto
repetitions=30
populationInitializationMode=1
printAtEveryEval=0
saveLogFilesOnEveryUpdate=0

seed=10

# optimizer=MO-GOMEA
# fos=learned
# IMS=1
# $path_to_executable -S $seed -P $path_to_project -I $IMS -e $evals -f $fit -F $fos -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate

# optimizer=NSGA-II
# variation=2p
# popsize=100
# $path_to_executable -S $seed -P $path_to_project -p $popsize -e $evals -f $fit -v $variation -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate
#
optimizer=MO-RS
$path_to_executable -S $seed -P $path_to_project -e $evals -f $fit -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate
#
# optimizer=MO-LS
# # maxnetworkunique=100000
# $path_to_executable -S $seed -P $path_to_project -e $evals -f $fit -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate
