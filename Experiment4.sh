#!/bin/bash
# path_to_executable=dist/Release/GNU-Linux/tomga
path_to_executable=out/GA-dxxucjfbkbbaercqnxcndhysuclb/Build/Products/Release/GA
path_to_project=src/../

# evals=10000000000
netuniqevals=50000
maxseconds=600
fit="ark8 14 2"
suffix="_ark8_cifar10"
prefix="Exp3_"
convergence=none
repetitions=30
populationInitializationMode=1
printAtEveryEval=0
saveLogFilesOnEveryUpdate=0

seed=10

# --------- EXPERIMENT 4: Extreme vs no extreme clusters
optimizer=MO-GOMEA
fos=learned
IMS=1
clusteramount=-1
for extremeclusters in 1 0
do
  $path_to_executable -S $seed -P $path_to_project -I $IMS -s $maxseconds -n $netuniqevals -f $fit -c $convergence -F $fos -C $clusteramount -X $extremeclusters -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate -O $prefix"extreme"$extremeclusters$suffix
done
