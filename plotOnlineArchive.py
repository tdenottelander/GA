import json
import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
import os
import sys

def FileCheck(fn):
    try:
        open(fn, "r")
        return True
    except IOError:
#         print("Error: File does not appear to exist.")
        return False

def plotArchive(ax, folder, network_unique_evaluations, color, label):
    filename = folder + "/progress0.json"
    if not FileCheck(filename):
        print("File with name", filename, "could not be found.")
        return

    with open(filename) as json_file:
        data = json.load(json_file)
        index = -1
        max_network_unique_evaluations = -1
        for i in range(len(data["network_unique_evals"])):
            max_network_unique_evaluations = data["network_unique_evals"][i]
            if data["network_unique_evals"][i] == network_unique_evaluations:
                index = i
                break

        if index == -1:
            print("Algorithm did not yet do", network_unique_evaluations, "evaluations. Can plot up to", max_network_unique_evaluations, "evaluations.")
            return

        archive = data["elitist_archive"][index]

        X = np.zeros(len(archive))
        Y = np.zeros(len(archive))

        for i in range(len(archive)):
            X[i] = archive[i]['f'][0]
            Y[i] = archive[i]['f'][1]

        ax.scatter(X, Y, color=color, label=label)

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
        elif "MO-RS" in filename:
            rsdir = prefix + filename
        elif "NSGA-II" in filename:
            nsgadir = prefix + filename

    print("MO-GOMEA:", mogomeadir)
    print("NSGA-II:", nsgadir)
    print("MO-LS:", lsdir)
    print("MO-RS:", rsdir)

    fig, ax = plt.subplots(figsize=(10,10))
    if mogomeadir is not "" and "MO-GOMEA" in algorithms_to_plot:
        plotArchive(ax, mogomeadir, network_unique_evaluations, color="red", label="MO-GOMEA")
    if nsgadir is not "" and "NSGA-II" in algorithms_to_plot:
        plotArchive(ax, nsgadir, network_unique_evaluations, color="green", label="NSGA-II")
    if lsdir is not "" and "MO-LS" in algorithms_to_plot:
        plotArchive(ax, lsdir, network_unique_evaluations, color="blue", label="MO-LS")
    if rsdir is not "" and "MO-RS" in algorithms_to_plot:
        plotArchive(ax, rsdir, network_unique_evaluations, color="orange", label="MO-RS")

    ax.legend()
    ax.grid()
    ax.set_title("Elitist archive after " + str(network_unique_evaluations) + " unique evaluations")
    ax.set_xlim(0.4, 0.8)
    ax.set_ylim(0.5, 1.0)
    ax.set_xlabel("Validation accuracy")
    ax.set_ylabel("normalized MMACs")
    plt.savefig("plotElitistArchive.png")
    # plt.show()

def main():
    evaluations = int(sys.argv[1])
    print("Showing elitist archive at", evaluations, "unique evaluations")

    algorithms = []
    for i in range(len(sys.argv) - 2):
        algorithms.append(str(sys.argv[i+2]))
    print("Showing archive for algorithms:", algorithms)

    plotAllOnThisServer(algorithms, evaluations)


if __name__ == '__main__':
    main()
