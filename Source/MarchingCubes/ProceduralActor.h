// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshBuilder.h"

#include "ProceduralActor.generated.h"

UCLASS()
class MARCHINGCUBES_API AProceduralActor : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AProceduralActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	float height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	float depth;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostActorCreated() override;

	virtual void PostInitProperties() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
#endif

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* mesh;

	void CreateTriangle();

	void LogTest(const TArray<FVector>& vertices, 
		const TArray<int32>& triangles, const TArray<FVector>& normals, const TArray<FVector2D>& uvs, 
		const TArray<FProcMeshTangent>& tangents, const TArray<FLinearColor>& vertexColors);

	TArray<FVector> _vertices;
	TArray<int32> _triangles;
	TArray<FVector> _normals;
	TArray<FVector2D> _uvs;
	TArray<FProcMeshTangent> _tangents;
	TArray<FLinearColor> _vertexColors;

	ProceduralMeshBuilder _procMeshHandler;

	bool _triggered;
	float _playTime;
};
