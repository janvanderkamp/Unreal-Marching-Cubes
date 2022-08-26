// Fill out your copyright notice in the Description page of Project Settings.


#include "MarchingCubeTestSingle.h"
#include "MarchingCubeUtils.h"
#include "PolygoniseUtils.h"

using namespace MarchingCubeUtils;
using namespace PolygoniseUtils;

// Sets default values
AMarchingCubeTestSingle::AMarchingCubeTestSingle() :
	InsideCorners(0),
	Isolevel(0),
	Size(1.f),
	_gridCell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("CubePolys"));

	RootComponent = _mesh;
	_mesh->bUseAsyncCooking = true;
}

// Called when the game starts or when spawned
void AMarchingCubeTestSingle::BeginPlay()
{
	Super::BeginPlay();

}

void AMarchingCubeTestSingle::PostActorCreated()
{
	Super::PostActorCreated();
}

void AMarchingCubeTestSingle::PostInitProperties()
{
	Super::PostInitProperties();
}

#if WITH_EDITOR
void AMarchingCubeTestSingle::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	// Just create a new WorldGridCell on each edit
	// Forcing all values to be less than Isolevel to flag that they start 
	// inside the surface by default
	_gridCell = WorldGridCell(FVector(1, 0, 0), Size, Isolevel - 1);
	UE_LOG(LogTemp, Warning, TEXT("PostEditChangeProperty: logging Size: %f, Isolevel: %f, InsideCorners: %d"), Size, Isolevel, InsideCorners);

	for (uint8 i = 0; i < 8; ++i)
	{
		uint8 compare = InsideCorners & 1 << i;
		if (InsideCorners & compare)
		{
			// Set the value for this corner to less than Isolevel to flag that it's outside the surface
			_gridCell.GridCell.values[i] = Isolevel + 1;
			UE_LOG(LogTemp, Warning, TEXT("Corner #%d is inside surface"), i+1);

		}
	}

	TArray<Triangle> triangles;
	Polygonise(_gridCell.GridCell, Isolevel, triangles);

	UE_LOG(LogTemp, Warning, TEXT("logging triangles:"));
	int index = 0;
	for (auto iter = triangles.begin(); iter != triangles.end(); ++iter)
	{
		UE_LOG(LogTemp, Log, TEXT("vert 1 - (%f, %f, %f)"), index++, (*iter).points[0].X, (*iter).points[0].Y, (*iter).points[0].Z);
		UE_LOG(LogTemp, Log, TEXT("vert 2 - (%f, %f, %f)"), index++, (*iter).points[1].X, (*iter).points[1].Y, (*iter).points[1].Z);
		UE_LOG(LogTemp, Log, TEXT("vert 3 - (%f, %f, %f)"), index++, (*iter).points[2].X, (*iter).points[2].Y, (*iter).points[2].Z);
	}

	CreateTriangles(triangles);


	Super::PostEditChangeProperty(propertyChangedEvent);
}
#endif

// Called every frame
void AMarchingCubeTestSingle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector newLocation = GetActorLocation();
	FRotator newRotation = GetActorRotation();
	float deltaRotation = DeltaTime * 10.f;
	newRotation.Yaw += deltaRotation;
	SetActorRotation(newRotation);
}

void AMarchingCubeTestSingle::CreateTriangles(const TArray<Triangle>& triangles)
{
	_procMeshBuilder.ResetBuffers(triangles.Num() * 3);
	for (auto& tri : triangles)
	{
		_procMeshBuilder.AddTriangle(tri.points[0], tri.points[1], tri.points[2]);
	}
	
	//LogTest(_procMeshBuilder._vertices, _procMeshBuilder._triangles, _procMeshBuilder._normals, _procMeshBuilder._UVs, _procMeshBuilder._tangents, _procMeshBuilder._vertexColors);
	_procMeshBuilder.LogTriangles();
	_mesh->CreateMeshSection_LinearColor(0, _procMeshBuilder._vertices, _procMeshBuilder._triangles, _procMeshBuilder._normals, _procMeshBuilder._UVs, _procMeshBuilder._vertexColors, _procMeshBuilder._tangents, true);

	// Enable collision data
	_mesh->ContainsPhysicsTriMeshData(true);
}


