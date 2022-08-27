// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshBuilder.h"
#include "MarchingCubeUtils.h"
#include "MarchingCubeTestSingle.generated.h"

// Forward declaration
namespace PolygoniseUtils
{
	struct Triangle;
}

/** This is for testing the polygonise functionality, and exposes a bitmask for toggling a combination of corners that are outside of the isosurface.
Triangles are then generated using ProceduralMeshBuilder for easy verification in editor **/
UCLASS()
class MARCHINGCUBES_API AMarchingCubeTestSingle : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AMarchingCubeTestSingle();

	/** Which corners should be set as inside the surface for this test **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "MarchingCubeUtils::EIndexedCorner"), Category = "World Grid")
	uint8 InsideCorners;

	/** Represents value that determines surface level, < isoLevel is inside, >= isoLevel is outside **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Grid")
	float Isolevel;

	/** How long the side of the cube should be when placed in scene **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Grid")
	float Size;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostActorCreated() override;

	virtual void PostInitProperties() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	void CreateTriangles(const TArray<PolygoniseUtils::Triangle>& triangles);

#if WITH_EDITOR

	// For drawing a cube to help vizualising triangle and corner positioning:
	// - draws a debug cube to show edges of the marching cube
	// - draws a small box with a color scaled by corner value: 
	//		- brighter than gray is outside the surface
	//		- darker than gray is inside the surface
	// - draws light lines showing edges of generated triangles
	void UpdateDebugCube(const TArray<PolygoniseUtils::Triangle>& triangles) const;
#endif

	// TODO: refactor common code out of this and WorldGrid
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* _mesh;

	ProceduralMeshBuilder _procMeshBuilder;

	MarchingCubeUtils::WorldGridCell _gridCell;
};
