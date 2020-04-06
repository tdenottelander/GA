#!/bin/bash
path_to_executable=dist/Release/GNU-Linux/tomga
# path_to_executable='/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/out/GA-dxxucjfbkbbaercqnxcndhysuclb/Build/Products/Debug/GA'
path_to_project=/export/scratch1/tdo/TomGA/
# path_to_project='/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/'

evals=1000000
# netuniqevals=10000
fit="ark8 14 1"
repetitions=10
populationInitializationMode=0
seed=10

suffix="_ark8_cifar10"

#Random Search
# optimizer=RS
# $path_to_executable -S $seed -P $path_to_project -e $evals -f $fit -o $optimizer -r $repetitions -M $populationInitializationMode -O $optimizer$suffix
#
# #Local Search
# order=rand
# for optimizer in LS LSS-0.01 LSS-0.05
# do
#   $path_to_executable -S $seed -P $path_to_project -e $evals -f $fit -o $optimizer $order -r $repetitions -M $populationInitializationMode -O $optimizer$suffix
# done

#GOM with IMS
optimizer=GOM
IMS=1
fos=learned
$path_to_executable -S $seed -P $path_to_project -I $IMS -e $evals -f $fit -F $fos -o $optimizer -r $repetitions -M $populationInitializationMode -O $optimizer"_"$fos$suffix

for fos in uni ark6
do
  for order in asc desc rand
  do
    $path_to_executable -S $seed -P $path_to_project -I $IMS -e $evals -f $fit -F $fos $order -o $optimizer -r $repetitions -M $populationInitializationMode -O $optimizer"_"$fos"_"$order$suffix
  done
done

for fos in IncrLT IncrLTR IncrLT_uni IncrLTR_uni IncrLTR_uniOrd
do
  $path_to_executable -S $seed -P $path_to_project -I $IMS -e $evals -f $fit -F $fos -o $optimizer -r $repetitions -M $populationInitializationMode -O $optimizer"_"$fos$suffix
done


#SimpleGA
optimizer=SimpleGA
for popsize in 50 100 200 500 1000
do
  for variation in 1p 2p 3p uni ark6
  do
    $path_to_executable -S $seed -P $path_to_project -p $popsize -e $evals -f $fit -v $variation -o $optimizer -r $repetitions -M $populationInitializationMode -O $optimizer"_pop"$popsize"_"$variation$suffix
  done
done
