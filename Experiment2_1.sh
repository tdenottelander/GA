#!/bin/bash
path_to_executable=out/GA-dxxucjfbkbbaercqnxcndhysuclb/Build/Products/Release/GA
path_to_project=src/../

# evals=10000000000
netuniqevals=50000
maxseconds=60
fit="ark8 14 2"
convergence=none
repetitions=30
populationInitializationMode=1
printAtEveryEval=0
saveLogFilesOnEveryUpdate=0
suffix="_ark8_cifar10"

seed=10



# --------- EXPERIMENT NO CLUSTERING (WITH IMS)
optimizer=MO-GOMEA
fos=learned
IMS=1
clusteramount=(-1 1 -1)
extremeclusters=(1 1 0)
for ((i = 0; i < 3; i++))
do
  # echo ${clusteramount[i]} ${extremeclusters[i]}
  clusters=${clusteramount[i]}
  extreme=${extremeclusters[i]}
  echo $clusters $extreme
  $path_to_executable -S $seed -P $path_to_project -I $IMS -s $maxseconds -n $netuniqevals -f $fit -c $convergence -F $fos -C $clusters -X $extreme -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate -O $optimizer"_clusters"$clusters"_extreme"$extreme$suffix
done
