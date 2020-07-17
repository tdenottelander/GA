#!/bin/bash
# path_to_executable=dist/Release/GNU-Linux/tomga
path_to_executable=out/GA-dxxucjfbkbbaercqnxcndhysuclb/Build/Products/Release/GA
path_to_project=src/../

# evals=10000000000
netuniqevals=50000
maxseconds=60
fit="ark7 14 2"
suffix="_ark7_cifar100"
convergence=none
repetitions=30
populationInitializationMode=1
printAtEveryEval=0
saveLogFilesOnEveryUpdate=0

seed=10



# --------- EXPERIMENT NO CLUSTERING (WITH IMS)
optimizer=MO-GOMEA
fos=learned
IMS=0
populationsize=(32 128 512)
clusters=(3 5 7    5 7 9    7 9 11)
for ((i = 0; i < 3; i++))
do
  # echo ${clusteramount[i]} ${extremeclusters[i]}
  pop=${populationsize[i]}
  for ((j = 0; j < 3; j++))
  do
    clus=${clusters[((i * 3 + j))]}
    echo $pop $clus
    $path_to_executable -S $seed -P $path_to_project -I $IMS -p $pop -s $maxseconds -n $netuniqevals -f $fit -c $convergence -F $fos -C $clus -o $optimizer -r $repetitions -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate -O $optimizer"_pop"$pop"_clusters"$clus$suffix
  done
done
