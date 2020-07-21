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

# --------- EXPERIMENT 3: effect of amount of clusters
optimizer=MO-GOMEA
fos=learned
IMS=1

for clusters in -1 1 2 3 4 5 6 7 8
do
   $path_to_executable -S $seed -P $path_to_project -I $IMS -s $maxseconds -n $netuniqevals -f $fit -c $convergence -F $fos -C $clusters -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate -O $prefix"clusters"$clusters$suffix
done
