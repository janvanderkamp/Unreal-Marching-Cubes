// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Lookup tables and routine from this article "Polygonising a scalar field":
// (Also see article for an excellent explanation of the approach)
// http://paulbourke.net/geometry/polygonise/
// (Code comments are my own)
namespace PolygoniseUtils
{
	struct Triangle
	{
		FVector points[3];
	};

    // Ordered list of points on a cube and the associated density values. Order is:
    // - Left Down Forward
    // - Right Down Forward
    // - Right Down Backward
    // - Left Down Backward
    // - Left Up Forward	
    // - Right Up Forward
    // - Left Up Backward
    // - Right Up Backward
    // TODO: since each GridCell stores the positions and values for each corner,
    // there are a lot of duplicated values. This should be optimised by looking up 
    // a 3D grid of positions and values
    struct GridCell
    {
        FVector points[8];
        float values[8];
    };

    // EdgeTable: a look-up table where the index into the array is an int representing the combination of corners
    // that are outside the surface. (Each bit that is set represents an ordered corner that is outside)
    // The integer values stored in the array represent the edges on the cube which contain a triangle vertex.
    int* EdgeTable();

    // Convenience type-def for 2D array to improve readability of
    // returning static triangle indices which make up lookup table
    typedef int(*TriLookupTable)[16];

    // TriTable: a look-up table where the index is an int with bits set for corners outside surface. (As with EdgeTable)
    // Each value in the array is an array of length 16 which represents triangle indices to be used 
    // when constructing triangles from vertices. Unused indices are indicated by filling the remaining values with -1
    //
    // E.g. there should be 2 triangles generated for the given index list: 
    // {1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
    TriLookupTable TriTable();

     
    /**
     *  Generate triangles for a given grid cell
     *  
     *  @param	gridCell		GridCell containing corner values to check isosurface on, 
     *                          and corner positions to interpolate between for triangle vertices
     *  @param	isolevel		isolevel determining border between inside / outside of surface
     *  @param	trianglesOut	array of triangles to append to
     *  @return                 number of triangles added by polygonising gridCell 
     */
    int Polygonise(const GridCell& gridCell, float isolevel, TArray<Triangle>& trianglesOut);


    /**
     *  Linearly interpolates between 2 vertices based on their associated values and a given value
     *
     *  @param	isolevel		value used for interpolating between valp1 and valp2
     *  @param	p1		        first vertex to interpolate between
     *  @param	p2		        second vertex to interpolate between
     *  @param	valp1       	first value used for interpolation
     *  @param	valp2       	second value used for interpolation
     *  @return                 interpolated vertex
     */
    FVector VertexInterp(float isolevel, const FVector& p1, const FVector& p2, float valp1, float valp2);
}

