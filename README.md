# Unreal-Marching-Cubes
Exploring Procedural Terrain using Marching Cubes in Unreal Engine 5

## Intro

This project demonstrates using the Marching Cubes algorithm for generating a triangular mesh based on an iso-surface using the ProceduralMeshComponent in Unreal Engine 5. It's a project I undertook to get to know the engine a bit and something I enjoyed working on a lot!

The Marching Cubes approach is based on assigning a value at each corner of a cube in 3D space, where a value greater than zero denotes a point outside a surface, and a value less than zero, a point inside the surface. 

Taking a single edge on the cube as an example, if one corner is inside the surface and one corner is outside, then that edge contains a triangle vertex that is slicing through the cube. If both corners are inside or outside, then no vertex is required. The point on the edge is determined through interpolation of the is-surface values at each corner. In the following image, the white corner has a value of **1**, and the remaining black corners have values of **-1**, resulting in 3 triangle vertices along edges shared by the 'white' corner:

<img src="https://github.com/janvanderkamp/Unreal-Marching-Cubes/blob/main/doc/example_corner.png" height="400">

Given a set of values for each corner, deriving the containing edges and ordered vertices for triangle generation is non-trivial. But these can be generated offline and included as lookup tables for both simplicity and speed.

For a more detailed explanation of the algorithm, along with both the look-up tables and short method for outputting a list of triangles, see  [^1]

The ma

## Future Work

- GPU [^1]
- Highly detailed meshes [^2]


A footnote can also have multiple lines[^9].  

### References:
[^1]: http://paulbourke.net/geometry/polygonise/ (1994)
[^2]: https://developer.nvidia.com/gpugems/gpugems3/part-i-geometry/chapter-1-generating-complex-procedural-terrains-using-gpu
- proc mesh

[^9]: Every new line should be prefixed with 2 spaces.  
  This allows you to have a footnote with multiple lines.