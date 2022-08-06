// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"

/**
 * 
 */
class MARCHINGCUBES_API ProceduralMeshBuilder
{
public:

	ProceduralMeshBuilder();
	~ProceduralMeshBuilder();

	void AddTriangle(const FVector& v1, const FVector& v2, const FVector& v3);

	void ResetBuffers(int numVerts);

public:

	TArray<FVector> _vertices;
	TArray<int32> _triangles;
	TArray<FVector> _normals;
	TArray<FVector2D> _UVs;
	TArray<FProcMeshTangent> _tangents;
	TArray<FLinearColor> _vertexColors;

	int _triangleNdx;
};
