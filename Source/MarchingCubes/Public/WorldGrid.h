// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WorldGridBuilder.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshBuilder.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldGrid.generated.h"

// Forward declaration
namespace PolygoniseUtils
{
	struct Triangle;
}

//USTRUCT(BlueprintType)
//struct FEquippedWeapons
//{
//	GENERATED_BODY()
//
//	class AWeapon* rifle = nullptr;
//	class AAnotherWeapon* pistol = nullptr;
//	
//	UPROPERTY(EditAnywhere)
//	int32 NumWeapons = 0;
//};

UCLASS()
class MARCHINGCUBES_API AWorldGrid : public AActor
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AWorldGrid();

	/** How many cubes will be marched in 3D space **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="World Grid")
	FIntVector Dimensions;

	/** List of rules for sampling noise for density function **/
	UPROPERTY(EditAnywhere, Category = "World Grid")
	TArray<FNoiseSampler> NoiseSampleParamsList;

	/** Scale final noise calculation by a single value **/
	UPROPERTY(EditAnywhere, Category = "World Grid")
	float AllNoiseScale;

	/** How long the side of each cube should be when placed in scene **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Grid")
	float Size;

	/** Represents value that determines surface level, < isoLevel is inside, >= isoLevel is outside **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Grid")	
	float Isolevel;

	/** Enable this to block grid from updating while making changes, disable when ready for grid to update **/
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "World Grid")
	bool LockUpdates;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	void CreateTriangles(const TArray<PolygoniseUtils::Triangle>& triangles);

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* _mesh;

	ProceduralMeshBuilder _procMeshBuilder;

	MarchingCubeUtils::WorldGridBuilder _worldGridBuilder;
};
