#!/bin/bash

# -?: print help
# -P [#1]: set project directory to #1  !!IMPORTANT THAT THIS IS DONE!!
# -S [#1]: set seed to #1 (any integer number)
# -e [#1]: set max evaluations to #1
# -u [#1]: set max unique evaluations to #1
# -n [#1]: set max network unique evaluations to #1
# -m [#1]: set max rounds to #1
# -s [#1]: set max seconds to #1
# -d [#1]: set dataset to #1={cifar10, cifar100} for ark-online
# -f [#1][#2][#3]: set fitness function to #1={zmom, lotz, tit, maxcut, ark1, ark2, ark3, ark4, ark5, ark6, ark7, ark-online, onemax, leadingones, trap, NK} with problemsize #2 and number of objectives #3
# -c [#1]: set convergence criteria to #1={entire_pareto, epsilon_pareto}
# -E [#1]: set epsilon to #1
# -F [#1][#2]: set FOS to #1={learned, uni, IncrLT, IncrLTR, IncrLT_uni, IncrLTR_uni, IncrLTR_uniOrd, triplet, tripletTree, ark6} with optional order #2={rand, asc, desc}
# -v [#1]: set variation operator to #1={1p, 2p, 3p, uni, ark6}
# -o [#1][#2]: set optimizer to #1={NSGA-II, MO-RS, MO-LS, MO-GOMEA, GOM, GOM-LS, RS, SimpleGA, LS, LSS-0.01, LSS-0.05} with optional order #2={rand, asc, desc}
# -r [#1]: set repetitions to #1
# -I [#1]: set IMS to #1={0,1}
# -p [#1]: set non-IMS Popsize to #1
# -i [#1]: set forced improvement to #1={0,1}
# -g [#1]: set genotype checking to #1={0,1}
# -l [#1]: set allow identity layers to #1={0,1}
# -M [#1]: set population initialization mode to #1={0 (true random), 1 (random, but first individual to all identity), 2 (solvable)}
# -a [#1]: set print full elitist archive to #1={0, 1}
# -q [#1]: set print every evaluation to #1={0, 1}
# -x [#1]: set saving log files on every update to #1={0, 1}
# Program ended with exit code: 0

path_to_executable=dist/Release/GNU-Linux/tomga
# path_to_executable='/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/out/GA-dxxucjfbkbbaercqnxcndhysuclb/Build/Products/Debug/GA'
path_to_project=src/../  #Will point to the current working directory
# path_to_project='/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/'

# networkUniqueEvals=1000
dataset="cifar100"
fit="ark-online 17 2"
repetitions=1
populationInitializationMode=1 #1 includes all-identity solution, 0 is true random
printAtEveryEval=1
saveLogFilesOnEveryUpdate=1
gpu=0
seed=$1

fos="learned"
optimizer="MO-GOMEA"
IMS=1
CUDA_VISIBLE_DEVICES=$gpu $path_to_executable -S $seed -P $path_to_project -d $dataset -f $fit -F $fos -o $optimizer -r $repetitions -I $IMS -M $populationInitializationMode -q $printAtEveryEval -x $saveLogFilesOnEveryUpdate
