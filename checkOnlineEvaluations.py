import glob
import json
import datetime

PREV_EVALS_FN = "prevEvaluations.json"

def FileCheck(fn):
    try:
        open(fn, "r")
        return True
    except IOError:
        return False

def checkEvaluations(folder, algo, prevEvals, maxruns=150, beginrun=0):
    for run in range(beginrun, maxruns):
        queriedFilename = folder + algo + "_run" + str(run+1) + "_seed*" + "/progress0.json"
        regexFilenames = glob.glob(queriedFilename)
        if len(regexFilenames) == 0:
#            print("File with name", queriedFilename, "could not be found.")
            break
        filename = regexFilenames[0]

        with open(filename) as json_file:
            data = json.load(json_file)
            evals = data["network_unique_evals"][-1]
            check = "" if evals < 2500 else "(done)"
            prevEval = getPrevEval(algo, run, prevEvals)
            increment = evals - prevEval
            plusString = ""
            if increment > 0:
                plusString = " (+"+str(evals - prevEval)+")"
            print(algo, "run", run+1, "is at", str(evals)+plusString, "unique_evaluations", check)
            putEval(algo, run, prevEvals, evals)

def getPrevEval(algo, run, prevEvals):
    key = algo + str(run)
    if key in prevEvals:
        return prevEvals[key]
    else:
        return 0

def putEval(algo, run, prevEvals, evals):
    key = algo + str(run)
    prevEvals[key] = evals

def savePrevEvals(prevEvals):
    with open(PREV_EVALS_FN, 'w') as outfile:
        json.dump(prevEvals, outfile)

def time(prevEvals):
    timestampNow = datetime.datetime.now()
    if "timestamp" in prevEvals:
        timestampPrev = datetime.datetime.strptime(prevEvals["timestamp"], '%a %b %d %H:%M:%S %Y')
        difference = timestampNow - timestampPrev
        print("Time since last retrieval:", difference.seconds//3600, "hours", (difference.seconds//60)%60, "minutes")
    prevEvals["timestamp"] = timestampNow.strftime('%a %b %d %H:%M:%S %Y')


def loadPreviousEvaluations():
    prevEvals = {}
    if FileCheck(PREV_EVALS_FN):
        with open (PREV_EVALS_FN) as json_file:
            data = json.load(json_file)
            for key in data.keys():
                prevEvals[key] = data[key]
    return prevEvals

prevEvals = loadPreviousEvaluations()

for algo in ["MO-GOMEA", "NSGA-II", "MO-LS", "MO-RS"]:
    checkEvaluations("data/online/", algo, prevEvals)
    print()

checkEvaluations("data/online/", "NSGA-II", prevEvals, beginrun=100)
print()

time(prevEvals)

savePrevEvals(prevEvals)
