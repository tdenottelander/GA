import matplotlib.pyplot as plt
import json
import numpy as np

problem = 'LeadingOnes'

filename = "../data/exp3/data_all.json"

fig, (ax1, ax2) = plt.subplots(1, 2)

with open(filename) as json_file:
    data = json.load(json_file)

#     fig = plt.figure()
    fig.suptitle(str(problem))

    experiments = data['experiments']

    numExperiments = len(experiments);
#     print("Experiments =", numExperiments);

    for exp in experiments:
        data = experiments[exp]
        numEntries = len(data)

        avgTimes = np.zeros(numEntries)
        stdTimes = np.zeros(numEntries)
        avgEvaluations = np.zeros(numEntries)
        stdEvaluations = np.zeros(numEntries)
        problemSizes = np.zeros(numEntries)

        for i, problemsize in enumerate(sorted(map(int, data))):
#             print(problemsize)
            problemSizes[i] = problemsize

            repetitions = data[str(problemsize)]
            times = np.empty(len(repetitions))
            evaluations = np.empty(len(repetitions))
            for j, rep in enumerate(repetitions):
                times[j] = repetitions[rep]['timeTaken']
                evaluations[j] = repetitions[rep]['evaluations']

            avgTimes[i] = np.mean(times)
            stdTimes[i] = np.std(times)
            avgEvaluations[i] = np.mean(evaluations)
            stdEvaluations[i] = np.std(evaluations)

        ax1.errorbar(problemSizes, avgTimes, yerr=stdTimes,
                     capsize=3, capthick=2, label=exp)
        ax2.errorbar(problemSizes, avgEvaluations, yerr=stdEvaluations,
                    capsize=3, capthick=2, label=exp)

    ax1.legend()
    ax2.legend()
    ax1.set_xlabel('problem size')
    ax2.set_xlabel('problem size')
    ax1.set_ylabel('time taken (ms)')
    ax2.set_ylabel('# evaluations')
    ax1.set_xscale('log')
    ax2.set_xscale('log')
    ax1.set_yscale('log')
    ax2.set_yscale('log')

#     y = [10**x for x in range(6)]
#     x = [2**x for x in range(2,9)]
#     plt.yticks(y, y)
#     plt.xticks(x, x)

plt.show();
