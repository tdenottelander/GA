#!/bin/bash
path_to_executable=dist/Release/GNU-Linux/tomga
path_to_project=src/../

evals=10000000
fit="ark8 14 2"
convergence=entire_pareto
repetitions=30
populationInitializationMode=1
printAtEveryEval=0
saveLogFilesOnEveryUpdate=0

seed=10

# optimizer=MO-GOMEA
# fos=learned
# IMS=1
# $path_to_executable -S $seed -P $path_to_project -I $IMS -e $evals -f $fit -c $convergence -F $fos -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate

#optimizer=NSGA-II
#variation=2p
#popsize=100
#$path_to_executable -S $seed -P $path_to_project -p $popsize -e $evals -f $fit -c $convergence -v $variation -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate
#
optimizer=MO-LS
maxnetworkunique=50000
$path_to_executable -S $seed -P $path_to_project -n $maxnetworkunique -e $evals -f $fit -c $convergence -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate

#optimizer=MO-RS
#$path_to_executable -S $seed -P $path_to_project -e $evals -f $fit -c $convergence -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate
