// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralMeshBuilder.h"
#include "MarchingCubeUtils.h"

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

// TODO: Remove duplicate vertices with appropriate index buffer 
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

	// TODO: average normals across triangles to improve appearance

	_vertexColors[_triangleNdx] = FLinearColor(0.75, 0.75, 0.75, 1.0);
	_vertexColors[_triangleNdx+1] = FLinearColor(0.75, 0.75, 0.75, 1.0);
	_vertexColors[_triangleNdx+2] = FLinearColor(0.75, 0.75, 0.75, 1.0);

	_triangleNdx += 3;
}

void ProceduralMeshBuilder::LogTriangles()
{
	UE_LOG(LogGrid, Warning, TEXT("logging vertices:"));
	int index = 0;
	for (auto iter = _vertices.begin(); iter != _vertices.end(); ++iter)
	{
		UE_LOG(LogGrid, Log, TEXT("%d - (%f, %f, %f)"), index++, (*iter).X, (*iter).Y, (*iter).Z);
	}

	UE_LOG(LogGrid, Warning, TEXT("logging triangles:"));
	index = 0;
	for (auto iter = _triangles.begin(); iter != _triangles.end(); ++iter)
	{
		UE_LOG(LogGrid, Log, TEXT("%d - (%d)"), index++, (*iter));
	}

	UE_LOG(LogGrid, Warning, TEXT("logging normals:"));
	index = 0;
	for (auto iter = _normals.begin(); iter != _normals.end(); ++iter)
	{
		UE_LOG(LogGrid, Log, TEXT("%d - (%f, %f, %f)"), index++, (*iter).X, (*iter).Y, (*iter).Z);
	}

	UE_LOG(LogGrid, Warning, TEXT("logging uvs:"));
	index = 0;
	for (auto iter = _UVs.begin(); iter != _UVs.end(); ++iter)
	{
		UE_LOG(LogGrid, Log, TEXT("%d - (%f, %f)"), index++, (*iter).X, (*iter).Y);
	}

	UE_LOG(LogGrid, Warning, TEXT("logging tangents:"));
	index = 0;
	for (auto iter = _tangents.begin(); iter != _tangents.end(); ++iter)
	{
		UE_LOG(LogGrid, Log, TEXT("%d - (%f, %f, %f), flipped: %s"), index++, (*iter).TangentX.X, (*iter).TangentX.Y, (*iter).TangentX.Z,
			(*iter).bFlipTangentY ? "true" : "false");
	}

	UE_LOG(LogGrid, Warning, TEXT("logging vertexColors:"));
	index = 0;
	for (auto iter = _vertexColors.begin(); iter != _vertexColors.end(); ++iter)
	{
		UE_LOG(LogGrid, Log, TEXT("%d - (%f, %f, %f)"), index++, (*iter).R, (*iter).G, (*iter).B);
	}
}
