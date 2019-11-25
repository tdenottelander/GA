import matplotlib.pyplot as plt
import json
import numpy as np

tournamentsize = 4
problem = 'leadingones'

filename = "../data/exp1/" + str(problem) + "_tour" + str(tournamentsize) + ".json"
with open(filename) as json_file:
    data = json.load(json_file)
    
    fig = plt.figure()
    fig.suptitle(str(problem) + " (with tournament(" + str(tournamentsize) + "))")
    
    for var in {'op', 'uni'}:
        data2 = data['sel=tour' + str(tournamentsize)]['var=' + var]
        numEntries = len(data2)
        avgTimes = np.zeros(numEntries)
        stdTimes = np.zeros(numEntries)
        problemSizes = np.zeros(numEntries)

        for i, problemsize in enumerate(sorted(map(int, data2))):

            problemSizes[i] = problemsize

            repetitions = data2[str(problemsize)]
            times = np.empty(30)
            for j, rep in enumerate(repetitions):
                time = repetitions[rep]['timeTaken']
                times[j] = time

            avgTimes[i] = np.mean(times)
            stdTimes[i] = np.std(times)

        plt.errorbar(problemSizes, avgTimes, yerr=stdTimes, 
                     capsize=3, capthick=2, label=var)
    
    plt.legend()
    plt.xlabel('problem size')
    plt.ylabel('time taken (ms)')
    plt.xscale('log')
    plt.yscale('log')
    
    y = np.array([1,10,100,1000,10000])
    x = np.array([4,8,16,32,64,128,256,512,1024])
    plt.yticks(y, y)
    plt.xticks(x, x)
    
    plt.show()