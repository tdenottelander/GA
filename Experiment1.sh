#!/bin/bash
path_to_executable=out/GA-dxxucjfbkbbaercqnxcndhysuclb/Build/Products/Release/GA
path_to_project=src/../

netuniqevals=50000
maxseconds=60
fit="ark7 14 2"
suffix="_ark7_cifar100"
prefix="Exp1_"
convergence=none
repetitions=30
populationInitializationMode=1
printAtEveryEval=0
saveLogFilesOnEveryUpdate=0

seed=10


# --------- EXPERIMENT 1 Fixed pops (and fixed number of clusters)
optimizer=MO-GOMEA
fos=learned

popsizes=(512 128 32 8)
clusteramount=(9 7 5 3)

for ((i = 0; i < 4; i++))
do
  popsize=${popsizes[i]}
  clusters=${clusteramount[i]}
  IMS=0

  $path_to_executable -S $seed -P $path_to_project -I $IMS -p $popsize -s $maxseconds -n $netuniqevals -f $fit -c $convergence -C $clusters -F $fos -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate -O $prefix$optimizer$suffix"_pop"$popsize"_seed"$seed
done

# --------- EXPERIMENT 1 IMS
# IMS=1
#
# $path_to_executable -S $seed -P $path_to_project -I $IMS -s $maxseconds -n $netuniqevals -f $fit -c $convergence -F $fos -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate -O $prefix$optimizer$suffix"_popIMS_seed"$seed
