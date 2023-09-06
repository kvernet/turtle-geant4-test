#!/usr/bin/env python3

from mulder import Direction, Fluxmeter, Grid, Position, Layer
import numpy
import matplotlib.colors as colors
import matplotlib.pyplot as plot

DataType = numpy.dtype(
    [
        ('azimuth', 'f8'),
        ('elevation', 'f8'),
        ('depth', 'f8')
    ],
    align=True
)

def g4turtleData(path):
    with open(path) as f:
        data = numpy.fromfile(f, dtype=DataType, count=-1)
    
    return data


def mulderData(azimuth, elevation, model, density=2.65):
    fluxmeter = Fluxmeter(
        Layer(
            material = "Rock",
            density = density * 1E+03, # kg / m^3
            model = model
        )
    )
    
    density = fluxmeter.geometry.layers[0].density

    """
        To get the corresponding height, use the display.py script and
        substract 100 in comparison to G4Turtle program.
        In that case height = 517.127 - 100
    """
    position = Position(
        latitude = 11.988726,
        longitude = -86.171573,
        height = 417.127
    )

    grid = Grid(
        azimuth = azimuth,
        elevation = elevation
    )
    direction = Direction(**grid.nodes)

    grammage = fluxmeter.grammage(
        position = position,
        direction = direction
    )

    # Note que les lignes de vue de la grille ont été 
    # applatie (flattened). Ainsi, grammage devrait avoir pour dimension (361 * 91,
    # 2). Soit grammage[:,0].reshape((91, 361)) correspond à l'épaisseur de roche
    # (et grammage[:,1] à  l'épaisseur d'air).
    
    return density, grammage


def plotData(azimuth, elevation, data, title, path):
    #plot.style.use("share/rock.mplstyle")
    
    for i, d in enumerate(data):
        plot.figure(figsize=(12, 5))
        plot.pcolormesh(azimuth, elevation, data[i],
                norm=colors.LogNorm(1E+01, 1E+03),
                cmap='terrain')
        
        plot.title(title[i])
        plot.xlabel("Azimuth [deg]")
        plot.ylabel("Elevation [deg]")
        plot.colorbar()
        plot.savefig(path[i])
    
    
    
    diff = data[1] - data[0]
    rdiff = 100 * (diff / data[0])
    plot.figure(figsize=(12, 5))
    plot.pcolormesh(azimuth, elevation, diff,
            cmap='terrain')
        
    plot.title("Turtle - G4Turtle [m]")
    plot.xlabel("Azimuth [deg]")
    plot.ylabel("Elevation [deg]")
    plot.colorbar()
    plot.savefig("share/plots/diff2D.png")
    
    
    plot.figure(figsize=(12, 5))
    plot.pcolormesh(azimuth, elevation, rdiff,
            cmap='terrain')
    
    plot.title("(Turtle - G4Turtle) / G4Turtle [%]")    
    plot.xlabel("Azimuth [deg]")
    plot.ylabel("Elevation [deg]")
    plot.colorbar()
    plot.savefig("share/plots/rdiff2D.png")
    
    

if __name__ == "__main__":
    data = g4turtleData("rock.turtle")
    
    azimuth = numpy.unique(tuple(data["azimuth"]))
    elevation = numpy.unique(tuple(data["elevation"]))
    shape = (len(azimuth), len(elevation))
    g4depth = data["depth"].reshape(shape).T
    
    model="share/topography/DEM_BACKGROUND_MASAYA.png"
    density, grammage = mulderData(
            azimuth=azimuth,
            elevation=elevation,
            model=model,
            density=2.65)
    
    shape = (len(elevation), len(azimuth))
    mdepth = grammage[:,0].reshape(shape) / density
       
    plotData(azimuth, elevation, [g4depth, mdepth],
            ["G4Turtle rock [m]", "Turtle rock [m]"],
            [
                "share/plots/g4turtle-rock.png",
                "share/plots/turtle-rock.png"
            ])
    
    
    
    plot.show()
    
    exit()
    
    
    
    az_range = [-180, 180]
    el_range = [0, 90]
    resolution = 1
    azimuth = numpy.arange(az_range[0], az_range[1] + resolution, resolution)
    elevation = numpy.arange(el_range[0], el_range[1] + resolution, resolution)
    
    density, grammage = \
            mulder_data(azimuth=azimuth,
                    elevation=elevation,
                    model="share/topography/DEM_BACKGROUND_MASAYA.png")
    
    shape = (len(elevation), len(azimuth))
    depth = grammage[:,0].reshape(shape) / density

    plot.figure(figsize=(12, 5))
    plot.pcolormesh(azimuth, elevation, depth,
            norm=colors.LogNorm(1E+01, 1E+03),
            cmap='terrain')
        
    plot.xlabel("Azimuth [deg]")
    plot.ylabel("Elevation [deg]")
    plot.colorbar()
    plot.savefig("share/plots/turtle-rock.png")

    plot.show()
