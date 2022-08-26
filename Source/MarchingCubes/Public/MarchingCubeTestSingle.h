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

	// TODO: refactor common code out of this and WorldGrid
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* _mesh;

	ProceduralMeshBuilder _procMeshBuilder;

	MarchingCubeUtils::WorldGridCell _gridCell;
};
