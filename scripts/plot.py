#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plot
import matplotlib.colors as colors

DataType = np.dtype(
    [
        ('azimuth', 'f8'),
        ('elevation', 'f8'),
        ('depth', 'f8')
    ],
    align=True
)

def getData(path):
    with open(path) as f:
        data = np.fromfile(f, dtype=DataType, count=-1)
    
    return data


def plotData(data):
    azimuth = np.unique(tuple(data["azimuth"]))
    elevation = np.unique(tuple(data["elevation"]))
    depth = data["depth"].reshape(len(azimuth), len(elevation))
    
    #plot.style.use("share/rock.mplstyle")
    plot.figure(figsize=(12, 5))
    plot.pcolormesh(azimuth, elevation, depth.T,
            norm=colors.LogNorm(1E+01, 1E+03),
            cmap='terrain')
    
    plot.xlabel("Azimuth [deg]")
    plot.ylabel("Elevation [deg]")
    plot.colorbar()
    plot.savefig("share/plots/g4turtle-rock.png")
    plot.show()


if __name__ == "__main__":
    data = getData("rock.turtle")
    
    plotData(data)
