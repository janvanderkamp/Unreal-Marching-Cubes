// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralMeshBuilder.h"

ProceduralMeshBuilder::ProceduralMeshBuilder()
{
}

ProceduralMeshBuilder::~ProceduralMeshBuilder()
{
}

void ProceduralMeshBuilder::ResetBuffers(int numVerts)
{
	_triangleNdx = 0;
	_vertices.SetNumUninitialized(numVerts);
	_triangles.SetNumUninitialized(numVerts);
	_normals.SetNumUninitialized(numVerts);
	_UVs.SetNumUninitialized(numVerts);
	_tangents.SetNumUninitialized(numVerts);
	_vertexColors.SetNumUninitialized(numVerts);
}

void ProceduralMeshBuilder::AddTriangle(const FVector& v1, const FVector& v2, const FVector& v3)
{
	// TODO: safety around duplicate points, and indexing past buffer size
	_vertices[_triangleNdx] = v1;
	_vertices[_triangleNdx +1] = v2;
	_vertices[_triangleNdx +2] = v3;

	_triangles[_triangleNdx] = _triangleNdx;
	_triangles[_triangleNdx +1] = _triangleNdx+1;
	_triangles[_triangleNdx +2] = _triangleNdx+2;

	FVector edge1 = v2 - v1;
	FVector edge2 = v2 - v3;

	_tangents[_triangleNdx] = FProcMeshTangent(edge1, false);
	_tangents[_triangleNdx + 1] = FProcMeshTangent(edge1, false);
	_tangents[_triangleNdx + 2] = FProcMeshTangent(edge1, false);

	//_UVs[_triangleNdx] = FVector2D(0, 0);
	//_UVs[_triangleNdx + 1] = FVector2D(0, 10);
	//_UVs[_triangleNdx + 2] = FVector2D(10, 0);

	// forgoing safety for speed here since 
	FVector normal = FVector::CrossProduct(edge1, edge2).GetUnsafeNormal();
	_normals[_triangleNdx] = normal;
	_normals[_triangleNdx+1] = normal;
	_normals[_triangleNdx+2] = normal;

	_vertexColors[_triangleNdx] = FLinearColor(0.75, 0.75, 0.75, 1.0);
	_vertexColors[_triangleNdx+1] = FLinearColor(0.75, 0.75, 0.75, 1.0);
	_vertexColors[_triangleNdx+2] = FLinearColor(0.75, 0.75, 0.75, 1.0);

	_triangleNdx += 3;
}