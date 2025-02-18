#!/bin/bash
# path_to_executable=dist/Release/GNU-Linux/tomga
path_to_executable=out/GA-dxxucjfbkbbaercqnxcndhysuclb/Build/Products/Release/GA
# path_to_project=src/../
path_to_project=`pwd`/
# echo $path_to_project

# evals=10000000000
netuniqevals=50000
maxseconds=180
fit="ark7 14 2"
convergence=none
repetitions=30
populationInitializationMode=1
printAtEveryEval=0
saveLogFilesOnEveryUpdate=0
suffix="_ark7_cifar100"

seed=10


# --------- EXPERIMENT DIFFERENT POPULATION SIZES (NO IMS)
optimizer=MO-GOMEA
fos=learned
IMS=0
# clustersize=9
for popsize in 512 256 128 64 32 16 8
do
  # echo clustersize $clustersize popsize $popsize
  $path_to_executable -S $seed -P $path_to_project -I $IMS -p $popsize -s $maxseconds -n $netuniqevals -f $fit -c $convergence -F $fos -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate -O $optimizer"_pop"$popsize$suffix
  # clustersize=$(($clustersize-1))
done


# --------- EXPERIMENT NO CLUSTERING (WITH IMS)
# optimizer=MO-GOMEA
# fos=learned
# IMS=1
# $path_to_executable -S $seed -P $path_to_project -I $IMS -p $popsize -n $netuniqevals -f $fit -c $convergence -F $fos -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate -O $optimizer"_pop"$popsize$suffix
