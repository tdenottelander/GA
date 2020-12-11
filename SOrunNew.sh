
fitfunc="ark7 14 1"
folderstuff="C100"
reps=100

# GOMEA
-S 10 -M 0 -n 100000 -s 60 -f $fitfunc -c optimal_fitness -F learned -I 1 -o GOM -r $reps -q 0 -O "SO_"$folderstuff"_GOMEA"

# LS
-S 10 -M 0 -n 100000 -s 60 -f $fitfunc -c optimal_fitness -o LS rand -r $reps -q 0 -O "SO_"$folderstuff"_LS"

# RS
-S 10 -M 0 -n 100000 -s 60 -f $fitfunc -c optimal_fitness -o RS -r $reps -q 0 -O "SO_"$folderstuff"_RS"

# GA
-S 10 -M 0 -n 100000 -s 60 -f $fitfunc -c optimal_fitness -v uni -o SimpleGA -r $reps -q 0 -O "SO_"$folderstuff"_GA"


# _GOMEA
-S 10 -M 0 -n 100000 -s 60 -f ark8 14 1 -c optimal_fitness -F learned -I 1 -o GOM -r 100 -q 0 -O SO_stuff

# LS
-S 10 -M 0 -n 100000 -s 60 -f ark8 14 1 -c optimal_fitness -o LS rand -r 100 -q 0 -O SO_stuff

# RS
-S 10 -M 0 -n 100000 -s 60 -f ark8 14 1 -c optimal_fitness -o RS -r 100 -q 0 -O SO_stuff


# GA
