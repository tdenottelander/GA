#!/bin/bash
# path_to_executable=dist/Release/GNU-Linux/tomga
path_to_executable=out/GA-dxxucjfbkbbaercqnxcndhysuclb/Build/Products/Release/GA
path_to_project=src/../

# evals=10000000000
netuniqevals=50000
maxseconds=60
fit="ark8 14 2"
suffix="_ark8_cifar10"
convergence=none
repetitions=30
populationInitializationMode=1
printAtEveryEval=0
saveLogFilesOnEveryUpdate=0

seed=10

# --------- EXPERIMENT 2.4: effect of different IMS cluster size schemes
optimizer=MO-GOMEA
fos=learned
IMS=1
for amountOfClusters in 4
#-1 1 3 5 7
do
   $path_to_executable -S $seed -P $path_to_project -I $IMS -s $maxseconds -n $netuniqevals -A $amountOfClusters -f $fit -c $convergence -F $fos -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate -O $optimizer"_startingamountofclusters"$amountOfClusters$suffix
done
