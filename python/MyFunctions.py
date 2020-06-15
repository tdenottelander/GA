import numpy as np

def calculateHyperVolume (np_archive, refX=0, refY=0):
    # Sorting array based on first objective
    sorted_archive = np_archive[np.argsort(np_archive[:, 0])]
    # Reverse array
    sorted_archive = sorted_archive[::-1]

    bottomBoundary = refX
    leftBoundary = refY

    hv = 0
    bottom = bottomBoundary
    for i in range(len(sorted_archive)):
        hv = hv + (sorted_archive[i, 0] - leftBoundary) * (sorted_archive[i, 1] - bottom)
        bottom = sorted_archive[i, 1]

    return hv
