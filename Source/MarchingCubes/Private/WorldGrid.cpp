// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGrid.h"

using namespace PolygoniseUtils;
using namespace MarchingCubeUtils;

// Sets default values
AWorldGrid::AWorldGrid() :
	Dimensions(FVector::ZeroVector),
	NoiseSampleParamsList(),
	AllNoiseScale(1.f),
	Size(1.f),
	Isolevel(1.f),
	LockUpdates(false),
	_mesh(nullptr),
	_procMeshBuilder(),
	_worldGridBuilder(Dimensions, Size)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("CubePolys"));

	RootComponent = _mesh;
	_mesh->bUseAsyncCooking = true;
}

// Called when the game starts or when spawned
void AWorldGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

#if WITH_EDITOR

/*
void AWorldGrid::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	// Just create a new one on each edit
	// Forcing all values to be greater than Isolevel to flag that they start 
	// outside the surface by default
	uint8 InsideCorners = 1;
	WorldGridCell _gridCell = WorldGridCell(FVector(1, 0, 0), Size, Isolevel + 1);
	UE_LOG(LogTemp, Warning, TEXT("PostEditChangeProperty: logging Size: %f, Isolevel: %f, InsideCorners: %d"), Size, Isolevel, InsideCorners);

	for (uint8 i = 0; i < 8; ++i)
	{
		uint8 compare = InsideCorners & 1 << i;
		if (InsideCorners & compare)
		{
			// Set the value for this corner to less than Isolevel to flag that it's inside the surface

			_gridCell.GridCell.values[i] = Isolevel - 1;
			UE_LOG(LogTemp, Warning, TEXT("Corner #%d is inside surface"), i + 1);

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

	//if ((InsideCorners | (uint8)EIndexedCorner::LeftDownBackward) == (uint8)EIndexedCorner::LeftDownBackward)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("only EIndexedCorner::LeftDownBackward is SET"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("something else is set"));
	//}

	Super::PostEditChangeProperty(propertyChangedEvent);
}
*/

void AWorldGrid::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	if (!LockUpdates)
	{
		// If no world grid properties changed, then skip grid creation
		//if (Dimensions != _worldGridBuilder.Dimensions() ||
		//	!FMath::IsNearlyEqual(Size, _worldGridBuilder.Size(), EPSILON))
		{
			// TODO: better encapsulation when updating World Grid Builder
			// TODO: also update if sample rules change

			// Set up grid with default values
			_worldGridBuilder.SetCells(Dimensions, Size);

			_worldGridBuilder.SetDensityValues(NoiseSampleParamsList, AllNoiseScale);

			TArray<Triangle> triangles;
			_worldGridBuilder.Polygonise(Isolevel, triangles);

			CreateTriangles(triangles);
		}
	}

	Super::PostEditChangeProperty(propertyChangedEvent);
}

#endif

// Called every frame
void AWorldGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorldGrid::CreateTriangles(const TArray<Triangle>& triangles)
{
	double start = FPlatformTime::Seconds();

	_procMeshBuilder.ResetBuffers(triangles.Num() * 3);
	for (auto& tri : triangles)
	{
		_procMeshBuilder.AddTriangle(tri.points[0], tri.points[1], tri.points[2]);
	}

	//_procMeshBuilder.LogTriangles();
	_mesh->CreateMeshSection_LinearColor(0, _procMeshBuilder._vertices, _procMeshBuilder._triangles, _procMeshBuilder._normals, _procMeshBuilder._UVs, _procMeshBuilder._vertexColors, _procMeshBuilder._tangents, true);

	// Enable collision data
	_mesh->ContainsPhysicsTriMeshData(true);

	double end = FPlatformTime::Seconds();
	UE_LOG(LogGrid, Log, TEXT("AWorldGrid::CreateTriangles: size: %d, took %f seconds"), triangles.Num(), end - start);
}

