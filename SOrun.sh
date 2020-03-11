#!/bin/bash
path_to_executable=dist/Release/GNU-Linux/tomga
# path_to_executable='/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/out/GA-dxxucjfbkbbaercqnxcndhysuclb/Build/Products/Debug/GA'
path_to_project=/export/scratch1/tdo/TomGA/
# path_to_project='/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/'

evals=10000000
fit="ark8 14 1"
repetitions=30
populationInitializationMode=0
IMS=0

#Random Search
optimizer=RS
$path_to_executable -P $path_to_project -I $IMS -p 10 -e $evals -f $fit -o $optimizer -r $repetitions -M $populationInitializationMode

for optimizer in LS LSS-0.01 LSS-0.05
do
  for order in asc desc rand
  do
    $path_to_executable -P $path_to_project -I $IMS -p 10 -e $evals -f $fit -o $optimizer $order -r $repetitions -M $populationInitializationMode
  done
done


#GOM without IMS
optimizer=GOM
for popsize in 50 100 200 500 1000
do
  fos=learned
  $path_to_executable -P $path_to_project -I $IMS -p $popsize -e $evals -f $fit -F $fos -o $optimizer -r $repetitions -M $populationInitializationMode

  for fos in uni ark6
  do
    for order in asc desc rand
    do
      $path_to_executable -P $path_to_project -I $IMS -p $popsize -e $evals -f $fit -F $fos $order -o $optimizer -r $repetitions -M $populationInitializationMode
    done
  done

  for fos in IncrLT IncrLTR IncrLT_uni IncrLTR_uni IncrLTR_uniOrd
  do
    $path_to_executable -P $path_to_project -I $IMS -p $popsize -e $evals -f $fit -F $fos -o $optimizer -r $repetitions -M $populationInitializationMode
  done
done

#SimpleGA
optimizer=SimpleGA
for popsize in 50 100 200 500 1000
do
  for variation in 1p 2p 3p uni ark6
  do
    $path_to_executable -P $path_to_project -I $IMS -p $popsize -e $evals -f $fit -v $variation -o $optimizer -r $repetitions -M $populationInitializationMode
  done
done
