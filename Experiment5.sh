#!/bin/bash
path_to_executable=out/GA-dxxucjfbkbbaercqnxcndhysuclb/Build/Products/Release/GA
path_to_project=src/../

netuniqevals=50000
maxseconds=60
fit="ark7 14 2"
suffix="_ark7_cifar100"
prefix="Exp5_"
convergence=none
repetitions=30
populationInitializationMode=1
printAtEveryEval=0
saveLogFilesOnEveryUpdate=0

seed=10


# --------- EXPERIMENT 5 different FOS types
optimizer=MO-GOMEA
fos=learned
IMS=1

for fos in learned RandLT uni IncrLT IncrLTR triplet
do
  $path_to_executable -S $seed -P $path_to_project -I $IMS -s $maxseconds -n $netuniqevals -f $fit -c $convergence -F $fos -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate -O $prefix$optimizer$suffix"_pop"$popsize"_seed"$seed
done
