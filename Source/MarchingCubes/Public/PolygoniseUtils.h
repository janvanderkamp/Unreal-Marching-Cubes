// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Lookup tables and routine from: http://paulbourke.net/geometry/polygonise/
// TODO: more descriptive comments
namespace PolygoniseUtils
{
	struct Triangle
	{
		FVector points[3];
	};

    struct GridCell
    {
        FVector points[8];
        float values[8];
    };

    int* EdgeTable();

    // Convenience type-def for 2D array to improve readability of
    // returning static triangle indices which make up lookup table
    typedef int(*TriLookupTable)[16];
    TriLookupTable TriTable();

	int Polygonise(const GridCell& grid, float isolevel, std::vector<Triangle>& trianglesOut);

	FVector VertexInterp(float isolevel, const FVector& p1, const FVector& p2, float valp1, float valp2);
}

