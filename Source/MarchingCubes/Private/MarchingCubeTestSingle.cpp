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
	TickInEditor(false),
	RotateSpeed(25.f),
	ShowDebugTriangles(false),
	_gridCell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#if WITH_EDITOR
	PrimaryActorTick.bStartWithTickEnabled = true;
#endif

	_mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("CubePolys"));

	RootComponent = _mesh;
	_mesh->bUseAsyncCooking = true;
}

AMarchingCubeTestSingle::~AMarchingCubeTestSingle()
{
#if WITH_EDITOR
	FlushPersistentDebugLines(GetWorld());
#endif
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
	PrimaryActorTick.bCanEverTick = true;

	// Just create a new WorldGridCell on each edit
	// Forcing all values to be less than 0 to flag that they start 
	// inside the surface by default
	_gridCell = WorldGridCell(FVector::ZeroVector, Size, -1);
	UE_LOG(LogTemp, Warning, TEXT("PostEditChangeProperty: logging Size: %f, Isolevel: %f, InsideCorners: %d"), Size, Isolevel, InsideCorners);

	for (uint8 i = 0; i < 8; ++i)
	{
		uint8 compare = InsideCorners & 1 << i;
		if (InsideCorners & compare)
		{
			// Set the value for this corner to more than 0 to flag that it's outside the surface
			_gridCell.GridCell.values[i] = 1;
			UE_LOG(LogTemp, Warning, TEXT("Corner #%d is inside surface"), i+1);

		}
	}

	TArray<Triangle> triangles;
	Polygonise(_gridCell.GridCell, Isolevel, triangles);

	CreateTriangles(triangles);

	UpdateDebugCube();

	Super::PostEditChangeProperty(propertyChangedEvent);
}

void AMarchingCubeTestSingle::UpdateDebugCube() const
{
	FRotator rot = GetActorRotation();

	FlushPersistentDebugLines(GetWorld());

	// Position and rotation for applying any transformations
	const FVector& pos = GetActorLocation();
	const FQuat& orientation = GetActorRotation().Quaternion();

	// First draw a debug cube to show edges where triangle vertices might be drawn
	UWorld * world = GetWorld();
	DrawDebugBox(world, pos, FVector(Size / 2.f), orientation, FColor(150, 150, 150), true, -1.f, 0, 0.5f);

	// Now draw a small box at each by taking corners local offset and applying Actor's rotation
	for (size_t i = 0; i < 8; i++)
	{
		const FVector& corner = orientation.RotateVector(_gridCell.GridCell.points[i]);
		double col = 255 * _gridCell.GridCell.values[i];
		DrawDebugBox(world, pos + corner, FVector(1), GetActorRotation().Quaternion(), FColor(col, col, col), true, -1.f, 0, 1.5f);
	}

#if WITH_EDITOR
	if (ShowDebugTriangles)
	{
		for (int32 i = 0; i < _procMeshBuilder._vertices.Num() - 1; ++i)
		{
			const FVector& vertA = orientation.RotateVector(_procMeshBuilder._vertices[i]);
			const FVector& vertB = orientation.RotateVector(_procMeshBuilder._vertices[i + 1]);
			DrawDebugLine(world, pos + vertA, pos + vertB, FColor(150, 150, 0), true, -1.f, 0, 0.5f);
		}
	}
#endif
}

#endif

// Called every frame
void AMarchingCubeTestSingle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator newRotation = GetActorRotation();
	float deltaRotation = DeltaTime * RotateSpeed;
	newRotation.Yaw += deltaRotation;

	SetActorRotation(newRotation);

#if WITH_EDITOR
	UpdateDebugCube();
#endif
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


