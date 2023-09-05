#!/usr/bin/env python3

from mulder import Grid, Layer
from mulder.matplotlib import LightSource, set_cursor_data

import numpy
import matplotlib.pyplot as plot

def display(model):
    layer = Layer(
        material = "Rock",
        density = 2.0E+03, # kg / m^3
        model = model,
    )
    
    print(f"""\
Map metadata:
- model:      {layer.model}
- projection: {layer.projection}
- nx:         {layer.nx}
- ny:         {layer.ny}
- xmin:       {layer.xmin}
- xmax:       {layer.xmax}
- ymin:       {layer.ymin}
- ymax:       {layer.ymax}
""")

    """
    x = 0.5 * (layer.xmin + layer.xmax)
    y = 0.5 * (layer.ymin + layer.ymax)
    latitude, longitude, height = layer.position(x, y)
    
    print(latitude, longitude, height)
    """
    
    grid = Grid(
        x = numpy.linspace(layer.xmin, layer.xmax, layer.nx),
        y = numpy.linspace(layer.ymax, layer.ymin, layer.ny)
    )

    # The height method returns interpolated height values of the topography, as

    z = layer.height(**grid.nodes)

    # In order to add specular effects to the drawing, we also need to compute the
    # outgoing normal to the topography surface. The later is obtained from the
    # gradient, as

    gx, gy = layer.gradient(**grid.nodes)
    normal = numpy.vstack((gx, gy, numpy.ones(z.size))).T

    # Following, we associate a set of colors to topography data using a LightSource
    # model. By default, colors are taken from a custom mulder.TerrainColormap,
    # with sea-level assumed to be at a height of zero.

    light = LightSource(
        intensity = 0.7,          # Intensity of ambiant light.
        direction = (-1, -1, -1)  # Light direction, for specular effects.
    )

    colors = light.colorize(
        z,
        normal,
        viewpoint = (-1, -1, 1),  # Optional viewpoint direction.
        cmap = None              # Optional as well, see comment above.
    )

    # The result needs to be recast in grid shape, for the plotting.

    colors = colors.reshape((*grid.shape, 4))

    # Finally, we plot the resulting picture.

    # plot.style.use("examples.mplstyle")
    plot.figure()
    image = plot.imshow(
        colors,
        origin="lower",
        extent=[layer.xmin, layer.xmax, layer.ymax, layer.ymin]
    )

    # This overrides cursor data such that hovering indicates actual height values
    # instead of colors.
    set_cursor_data(image, grid.reshape(z))
    
    latitude = 11.988726
    longitude = -86.171573
    projection = layer.project(latitude, longitude, 500)
    plot.plot(projection.x, projection.y, "ro")
    
    height = layer.height(projection)
    print(f"(x, y, z) = ({projection.x}, {projection.y}, {height:.3f})")

    plot.xlabel("longitude (deg)")
    plot.ylabel("latitude (deg)")
    plot.show()

if __name__ == "__main__":
    display("share/topography/DEM_BACKGROUND_MASAYA.png")
