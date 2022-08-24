// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PolygoniseUtils.h" 
#include "MarchingCubeUtils.generated.h" 

// TODO: enums and log declarations in own header?
DECLARE_LOG_CATEGORY_EXTERN(LogGrid, Log, All)

USTRUCT(BlueprintType)
struct FNoiseSampler
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Frequency = 0.f;

	UPROPERTY(EditAnywhere)
	float Amplitude = 0.f;
};


namespace MarchingCubeUtils
{
	const float EPSILON = .00000001f;

	UENUM(BlueprintType, meta = (UseEnumFlagsAsMaskValuesInEditor = "true"))
		enum class EIndexedCorner : uint8
	{
		LeftDownForwardVector	= 1 << 0,
		RightDownForwardVector	= 1 << 1,
		RightDownBackwardVector = 1 << 2,
		LeftDownBackwardVector	= 1 << 3,
		LeftUpForwardVector		= 1 << 4,
		RightUpForwardVector	= 1 << 5,
		LeftUpBackwardVector	= 1 << 6,
		RightUpBackwardVector	= 1 << 7,
		None = 0
	};

	// The purpose of WorldGridCell is to take in positions and a size in world space,
	// and to construct a GridCell with the resulting corner offsets.
	// It also exposes setting the iso-level value at each corner.
	// TODO: Optimizing the storage of corners / values will also vastly improve the 
	// performance of generating a large grid by removing redundant calls 
	// to set iso-level value at each corner
	struct WorldGridCell
	{
		static const FVector LeftDownForwardVector;
		static const FVector RightDownForwardVector;
		static const FVector RightDownBackwardVector;
		static const FVector LeftDownBackwardVector;
		static const FVector LeftUpForwardVector;
		static const FVector RightUpForwardVector;
		static const FVector RightUpBackwardVector;
		static const FVector LeftUpBackwardVector;

		// TODO: disable any ctors that aren't needed
		WorldGridCell(const FVector& position = FVector::ZeroVector, float size=1.f, float initialIsoLevelValue=0.f) :

			// Direct initialisation of aggregate struct GridCell
			GridCell {
				
				// Initial Corner Offsets by scaling a unit cube by size
				{ 
					LeftDownForwardVector	*	size + position,
					RightDownForwardVector	*	size + position,
					RightDownBackwardVector	*	size + position,
					LeftDownBackwardVector	*	size + position,
					LeftUpForwardVector		*	size + position,
					RightUpForwardVector	*	size + position,
					RightUpBackwardVector	*	size + position,
					LeftUpBackwardVector	*	size + position
				},

				// Default value of iso level for corners in same order as offsets
				{	
					initialIsoLevelValue,	// LeftDownForward	
					initialIsoLevelValue,	// RightDownForward	
					initialIsoLevelValue, 	// RightDownBackward	
					initialIsoLevelValue, 	// LeftDownBackward	
					initialIsoLevelValue, 	// LeftUpForward		
					initialIsoLevelValue, 	// RightUpForward	
					initialIsoLevelValue, 	// RightUpBackward	
					initialIsoLevelValue 	// LeftUpBackward	
				}
			}
		{
			// UE_LOG(LogGrid, Log, TEXT("WorldGridCell::ctor - Center: (%s)"), *position.ToString())
		}

		void LogCell()
		{
			for (size_t i = 0; i < 8; i++)
			{
				UE_LOG(LogGrid, Log, TEXT("WorldGridCell::LogCell - Corner (%d), point: (%s), value: %f"), i, *GridCell.points[i].ToString(), GridCell.values[i]);
			}
		}

		PolygoniseUtils::GridCell GridCell;
	};
}
