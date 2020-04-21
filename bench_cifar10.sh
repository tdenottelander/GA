#!/bin/bash
path_to_executable=dist/Release/GNU-Linux/tomga
path_to_project=src/../

# evals=10000000000
netuniqevals=100000
maxseconds=3600
fit="ark8 14 2"
convergence=none
repetitions=30
populationInitializationMode=1
printAtEveryEval=0
saveLogFilesOnEveryUpdate=0
suffix="_ark8_cifar10"

seed=10

optimizer=MO-GOMEA
fos=learned
IMS=1
$path_to_executable -S $seed -P $path_to_project -I $IMS -n $netuniqevals -f $fit -c $convergence -F $fos -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate -O $optimizer$suffix

# optimizer=NSGA-II
# variation=2p
# for popsize in 100 200 400 800
# do
# popsize=100
# $path_to_executable -S $seed -P $path_to_project -p $popsize -s $maxseconds -n $netuniqevals -f $fit -c $convergence -v $variation -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate -O $optimizer"_final"$suffix
# done

# optimizer=MO-RS
# $path_to_executable -S $seed -P $path_to_project -n $netuniqevals -f $fit -c $convergence -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate -O $optimizer$suffix
#
# optimizer=MO-LS
# $path_to_executable -S $seed -P $path_to_project -n $netuniqevals -f $fit -c $convergence -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate -O $optimizer$suffix
