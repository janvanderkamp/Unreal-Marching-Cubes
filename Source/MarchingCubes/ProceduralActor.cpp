// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralActor.h"

// Sets default values
AProceduralActor::AProceduralActor() : 
	width(100), 
	height(100)
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

	UE_LOG(LogTemp, Warning, TEXT("Triangle Corners - (%f, %f)"), width, height);
}

#if WITH_EDITOR

void AProceduralActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("Triangle Corners - (%f, %f)"), width, height);
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
}

void AProceduralActor::CreateTriangle()
{
	vertices.Empty();
	vertices.Add(FVector(0, 0, 0));
	vertices.Add(FVector(0, width, 0));
	vertices.Add(FVector(0, 0, height));

	triangles.Empty();
	triangles.Add(0);
	triangles.Add(1);
	triangles.Add(2);

	normals.Empty();
	normals.Add(FVector(1, 0, 0));
	normals.Add(FVector(1, 0, 0));
	normals.Add(FVector(1, 0, 0));

	UV0.Empty();
	UV0.Add(FVector2D(0, 0));
	UV0.Add(FVector2D(10, 0));
	UV0.Add(FVector2D(0, 10));


	tangents.Empty();
	tangents.Add(FProcMeshTangent(0, 1, 0));
	tangents.Add(FProcMeshTangent(0, 1, 0));
	tangents.Add(FProcMeshTangent(0, 1, 0));

	vertexColors.Empty();
	vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));

	mesh->CreateMeshSection_LinearColor(0, vertices, triangles, normals, UV0, vertexColors, tangents, true);

	// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);
}


