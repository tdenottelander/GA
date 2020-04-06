#!/bin/bash
# path_to_executable=dist/Release/GNU-Linux/tomga
path_to_executable='/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/out/GA-dxxucjfbkbbaercqnxcndhysuclb/Build/Products/Release/GA'
# path_to_project=/export/scratch1/tdo/TomGA/
# path_to_project='/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/'
path_to_project='src/../'

evals=10000000
netUniqEvals=100000
fit="ark7 14 1" #Cifar10
# fit="ark7 14 1" #Cifar100
suffix="_ark7_cifar100"
repetitions=30
populationInitializationMode=0
seed=10

#Random Search
optimizer=RS
$path_to_executable -S $seed -P $path_to_project -e $evals -n $netUniqEvals -f $fit -o $optimizer -r $repetitions -M $populationInitializationMode -O $optimizer$suffix

# for optimizer in LS LSS-0.01 LSS-0.05
# do
#   for order in rand
#   do
#     $path_to_executable -S $seed -P $path_to_project -e $evals -n $netUniqEvals -f $fit -o $optimizer $order -r $repetitions -M $populationInitializationMode -O $optimizer"_"$variation$suffix
#   done
# done

optimizer=GOM
fos=learned
IMS=1
$path_to_executable -S $seed -P $path_to_project -I $IMS -e $evals -n $netUniqEvals -f $fit -F $fos -o $optimizer -r $repetitions -M $populationInitializationMode -O $optimizer"_"$fos$suffix


#GOM without IMS
# optimizer=GOM
# for popsize in 50 100 200 500 1000
# do
#   fos=learned
#   $path_to_executable -P $path_to_project -I $IMS -p $popsize -e $evals -f $fit -F $fos -o $optimizer -r $repetitions -M $populationInitializationMode
#
#   for fos in uni ark6
#   do
#     for order in asc desc rand
#     do
#       $path_to_executable -P $path_to_project -I $IMS -p $popsize -e $evals -f $fit -F $fos $order -o $optimizer -r $repetitions -M $populationInitializationMode
#     done
#   done
#
#   for fos in IncrLT IncrLTR IncrLT_uni IncrLTR_uni IncrLTR_uniOrd
#   do
#     $path_to_executable -P $path_to_project -I $IMS -p $popsize -e $evals -f $fit -F $fos -o $optimizer -r $repetitions -M $populationInitializationMode
#   done
# done

#SimpleGA
# optimizer=SimpleGA
# for popsize in 50 100 200 400 800
# do
#   for variation in 2p #1p 2p 3p uni ark6
#   do
    # $path_to_executable -S $seed -P $path_to_project -p $popsize -e $evals -n $netUniqEvals -f $fit -v $variation -o $optimizer -r $repetitions -M $populationInitializationMode -O $optimizer"Mut_pop"$popsize"_"$variation$suffix
#   done
# done
