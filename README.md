# Unreal-Marching-Cubes
Exploring Procedural Terrain using Marching Cubes in Unreal Engine 5

## Intro

This project demonstrates using the Marching Cubes algorithm for generating a triangular mesh based on an iso-surface using the ProceduralMeshComponent in Unreal Engine 5. It's a project I undertook to get to know the engine a bit and something I enjoyed working on a lot!

The Marching Cubes approach is based on assigning a value at each corner of a cube in 3D space, where a value greater than zero denotes a point outside a surface, and a value less than zero, a point inside the surface. 

Taking a single edge on the cube as an example, if one corner is inside the surface and one corner is outside, then that edge contains a triangle vertex that is slicing through the cube. If both corners are inside or outside, then no vertex is required. The point on the edge is determined through interpolation of the is-surface values at each corner. In the following image, the white corner has a value of **1**, and the remaining black corners have values of **-1**, resulting in 3 triangle vertices along edges shared by the 'white' corner:

<img src="https://github.com/janvanderkamp/Unreal-Marching-Cubes/blob/main/doc/example_corner.png" height="400">

Given a set of values for each corner, deriving the containing edges and ordered vertices for triangle generation is non-trivial. But these can be generated offline and included as lookup tables for both simplicity and speed.

For a more detailed explanation of the algorithm, along with both the look-up tables and short method for outputting a list of triangles, see article *Polygonising a scalar field* [^1]. These have been adapted and included here, see [PolygoniseUtils.h](Source/MarchingCubes/Public/PolygoniseUtils.h)/[.cpp](Source/MarchingCubes/Private/PolygoniseUtils.cpp)

After generating triangle / index buffers etc. (using [^3] as an example) I added some editor tooling to toggle which corners are outside of the surface. By using an enum as a bitmask to represent the corners, each one can be toggled on / off.

Along with debug drawing to indicate cube edges/corners and triangle edges, it's pretty easy to test this visually in-editor:

<img src="https://github.com/janvanderkamp/Unreal-Marching-Cubes/blob/main/doc/toggle_corners.gif" height="400">

As can be seen here, depending on the combination of corners that are inside / outside, a large variety of shapes can be generated (max 5 triangles per cube):

<img src="https://github.com/janvanderkamp/Unreal-Marching-Cubes/blob/main/doc/rotate_toggle.gif" height="400">

We can even increase / decrease the iso-level value (which determines the edge of the surface and the points to generate triangles along cube edges):

<img src="https://github.com/janvanderkamp/Unreal-Marching-Cubes/blob/main/doc/isolevel.gif" height="400">

## Larger Surfaces

With functionality tested on a single cube, it's time to test this on a larger grid. By placing many cubes in a 3D space and supplying a single density function, we can generate a surface that runs through this space.

A simple sphere equation:

```
double WorldGridBuilder::DensityFunctionTest(const FVector& corner, const TArray<FNoiseSampler>& noiseSamplers)
{
	double density = 0.f;

	FVector sample(corner.X / _dimensions.X, corner.Y / _dimensions.Y, corner.Z / _dimensions.Z);
	FVector toCenter = FVector(0.5f) - sample;

	density += toCenter.SquaredLength();

	return density;
}
```
<img src="https://github.com/janvanderkamp/Unreal-Marching-Cubes/blob/main/doc/sphere.gif" height="400">

(not yet averaging normals, so triangle edges are quite pronounced)


## Procedural Surface with Perlin Noise

### TODO: add gifs and explanation 

Show how can use using successive octaves of noise to generate interesting surfaces

## Future Work

- Run on GPU: it's way too slow on CPU to experiment with the terrain generation
- Average normals for better rendering
- Share triangle vertices
- Highly detailed meshes

### References:
[^1]: Paul Bourke: Polygonising a scalar field (1994)  
  http://paulbourke.net/geometry/polygonise/
[^2]: Ryan Geiss, nvidia: Generating Complex Procedural Terrains Using GPU (2007)  
  https://developer.nvidia.com/gpugems/gpugems3/part-i-geometry/chapter-1-generating-complex-procedural-terrains-using-gpu
[^3]: Procedural Mesh Component in C++:Getting Started  
  https://unrealcommunity.wiki/procedural-mesh-component-in-cpp:getting-started-nfj6pimv

