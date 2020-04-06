import glob
import json

def checkEvaluations(folder, algo, maxruns=6):
    for run in range(maxruns):
        queriedFilename = folder + algo + "_run" + str(run+1) + "_seed*" + "/progress0.json"
        regexFilenames = glob.glob(queriedFilename)
        if len(regexFilenames) == 0:
            print("File with name", queriedFilename, "could not be found.")
            break
        filename = regexFilenames[0]

        with open(filename) as json_file:
            data = json.load(json_file)
            print(algo, "run", run+1, "is at", str(data["network_unique_evals"][-1]), "unique_evaluations")

for algo in ["MO-GOMEA", "NSGA-II", "MO-LS", "MO-RS"]:
    checkEvaluations("data/online/", algo)
    print()
