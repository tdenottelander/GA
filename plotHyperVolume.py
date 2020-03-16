import json
import numpy as np
import os
import matplotlib as mpl
if os.environ.get('DISPLAY','') == '':
    print('no display found. Using non-interactive Agg backend')
    mpl.use('Agg')
import matplotlib.pyplot as plt
import sys

def FileCheck(fn):
    try:
        open(fn, "r")
        return True
    except IOError:
#         print("Error: File does not appear to exist.")
        return False

def calculateHypervolume(archive):
    nparchive = np.zeros((len(archive), 2))
    for i, sol in enumerate(archive):
        nparchive[i, 0] = sol["f"][0]
        nparchive[i, 1] = sol["f"][1]

    # Sorting array based on first objective
    sorted_archive = nparchive[np.argsort(nparchive[:, 0])]
    # Reverse array
    sorted_archive = sorted_archive[::-1]

    bottomBoundary = 0
    leftBoundary = 0

    hv = 0
    bottom = bottomBoundary
    for i in range(len(sorted_archive)):
        hv = hv + (sorted_archive[i, 0] - leftBoundary) * (sorted_archive[i, 1] - bottom)
        bottom = sorted_archive[i, 1]

    return hv

def plotHyperVolume(ax, folder, network_unique_evaluations, color, label):
    filename = folder + "/progress0.json"
    if not FileCheck(filename):
        print("File with name", filename, "could not be found.")
        return

    with open(filename) as json_file:
        data = json.load(json_file)

        unique_evals = -1
        new_unique_evals = -1
        max_network_unique_evaluations = -1

        hypervolume = np.zeros(network_unique_evaluations)

        for i in range(len(data["network_unique_evals"])):
            new_unique_evals = data["network_unique_evals"][i]
            if new_unique_evals == unique_evals:
                continue
            unique_evals = new_unique_evals
            max_network_unique_evaluations = unique_evals

            archive = data["elitist_archive"][i]
            hv = calculateHypervolume(archive)
            hypervolume[unique_evals - 1] = hv

            if max_network_unique_evaluations == network_unique_evaluations:
                break

        ax.plot(list(range(len(hypervolume))), 1 - hypervolume, color=color, label=label)

def plotAllOnThisServer (algorithms_to_plot, network_unique_evaluations):
    mogomeadir = ""
    lsdir = ""
    rsdir = ""
    nsgadir = ""

    prefix = "data/"

    for filename in sorted(os.listdir(prefix)):
        if "MO-GOMEA" in filename:
            mogomeadir = prefix + filename
        elif "MO-LS" in filename:
            lsdir = prefix + filename
        elif "MO-RandomSearch" in filename:
            rsdir = prefix + filename
        elif "NSGA-II" in filename:
            nsgadir = prefix + filename

    print("MO-GOMEA:", mogomeadir)
    print("NSGA-II:", nsgadir)
    print("MO-LS:", lsdir)
    print("MO-RS:", rsdir)

    fig, ax = plt.subplots(figsize=(10,10))
    if mogomeadir is not "" and "MO-GOMEA" in algorithms_to_plot:
        plotHyperVolume(ax, mogomeadir, network_unique_evaluations, color="red", label="MO-GOMEA")
    if nsgadir is not "" and "NSGA-II" in algorithms_to_plot:
        plotHyperVolume(ax, nsgadir, network_unique_evaluations, color="green", label="NSGA-II")
    if lsdir is not "" and "MO-LS" in algorithms_to_plot:
        plotHyperVolume(ax, lsdir, network_unique_evaluations, color="blue", label="MO-LS")
    if rsdir is not "" and "MO-RS" in algorithms_to_plot:
        plotHyperVolume(ax, rsdir, network_unique_evaluations, color="orange", label="MO-RS")

    ax.legend()
    ax.grid()
    ax.set_title("HyperVolume until " + str(network_unique_evaluations) + " unique evaluations")
    ax.set_xlim(1, network_unique_evaluations)
    # ax.set_ylim(0.5, 1.0)
    ax.set_xlabel("Unique evaluations")
    ax.set_ylabel("(1 - HyperVolume)")
    ax.set_xscale("log")
    ax.set_yscale("log")
    plt.savefig("plotHyperVolume.png")
    print("Saving plot to 'plotHyperVolume.png' in current working directory.")
    # plt.show()

def main():
    evaluations = int(sys.argv[1])
    print("Plotting hypervolume until", evaluations, "unique evaluations")

    algorithms = []
    for i in range(len(sys.argv) - 2):
        algorithms.append(str(sys.argv[i+2]))
    print("Plotting hypervolume for algorithms:", algorithms)

    plotAllOnThisServer(algorithms, evaluations)


if __name__ == '__main__':
    main()
