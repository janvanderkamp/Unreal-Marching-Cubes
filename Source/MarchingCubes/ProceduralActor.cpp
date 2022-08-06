// Fill out your copyright notice in the Description page of Project Settings.
#include "ProceduralActor.h"

// Sets default values
AProceduralActor::AProceduralActor() :
	height(100),
	depth(100),
	_playTime(0),
	_triggered(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;

	mesh->bUseAsyncCooking = true;

	UE_LOG(LogTemp, Warning, TEXT("Hello from ctor!"));
}

void AProceduralActor::PostActorCreated()
{
	Super::PostActorCreated();

	CreateTriangle();
}

void AProceduralActor::PostInitProperties()
{
	Super::PostInitProperties();

	UE_LOG(LogTemp, Warning, TEXT("Triangle Corners - (%f, %f)"), height, depth);
}

#if WITH_EDITOR

void AProceduralActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("Triangle Corners - (%f, %f)"), height, depth);
	CreateTriangle();

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif

// Called when the game starts or when spawned
void AProceduralActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProceduralActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_playTime += DeltaTime;
	if (_playTime >= 5.f && !_triggered)
	{
		UE_LOG(LogTemp, Error, TEXT("TRIGGERED!"));
		CreateTriangle();
		_triggered = true;
	}
}

void AProceduralActor::CreateTriangle()
{
	UE_LOG(LogTemp, Warning, TEXT("logging custom:"));
	_procMeshHandler.ResetBuffers(6);
	_procMeshHandler.AddTriangle(FVector(0, 0, 0), FVector(0, height, 0), FVector(0, 0, depth));
	_procMeshHandler.AddTriangle(FVector(0, height, 0), FVector(0, height, depth), FVector(0, 0, depth));

	LogTest(_procMeshHandler._vertices, _procMeshHandler._triangles, _procMeshHandler._normals, _procMeshHandler._UVs, _procMeshHandler._tangents, _procMeshHandler._vertexColors);
	mesh->CreateMeshSection_LinearColor(0, _procMeshHandler._vertices, _procMeshHandler._triangles, _procMeshHandler._normals, _procMeshHandler._UVs, _procMeshHandler._vertexColors, _procMeshHandler._tangents, true);

	// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);
}

void AProceduralActor::LogTest(const TArray<FVector>& vertices, const TArray<int32>& triangles, const TArray<FVector>& normals, 
							   const TArray<FVector2D>& uvs, const TArray<FProcMeshTangent>& tangents, const TArray<FLinearColor>& vertexColors)
{
	UE_LOG(LogTemp, Warning, TEXT("logging vertices:"));
	int index = 0;
	for (auto iter = vertices.begin(); iter != vertices.end(); ++iter)
	{
		UE_LOG(LogTemp, Log, TEXT("%d - (%f, %f, %f)"), index++, (*iter).X, (*iter).Y, (*iter).Z);
	}

	UE_LOG(LogTemp, Warning, TEXT("logging triangles:"));
	index = 0;
	for (auto iter = triangles.begin(); iter != triangles.end(); ++iter)
	{
		UE_LOG(LogTemp, Log, TEXT("%d - (%d)"), index++, (*iter));
	}

	UE_LOG(LogTemp, Warning, TEXT("logging normals:"));
	index = 0;
	for (auto iter = vertices.begin(); iter != vertices.end(); ++iter)
	{
		UE_LOG(LogTemp, Log, TEXT("%d - (%f, %f, %f)"), index++, (*iter).X, (*iter).Y, (*iter).Z);
	}

	UE_LOG(LogTemp, Warning, TEXT("logging uvs:"));
	index = 0;
	for (auto iter = vertices.begin(); iter != vertices.end(); ++iter)
	{
		UE_LOG(LogTemp, Log, TEXT("%d - (%f, %f, %f)"), index++, (*iter).X, (*iter).Y);
	}

	UE_LOG(LogTemp, Warning, TEXT("logging tangents:"));
	index = 0;
	for (auto iter = vertices.begin(); iter != vertices.end(); ++iter)
	{
		UE_LOG(LogTemp, Log, TEXT("%d - (%f, %f, %f)"), index++, (*iter).X, (*iter).Y, (*iter).Z);
	}

	UE_LOG(LogTemp, Warning, TEXT("logging vertexColors:"));
	index = 0;
	for (auto iter = vertices.begin(); iter != vertices.end(); ++iter)
	{
		UE_LOG(LogTemp, Log, TEXT("%d - (%f, %f, %f)"), index++, (*iter).X, (*iter).Y, (*iter).Z);
	}
}

