#include "MarchingCubeUtils.h"
//#include "FMyInterpStruct.generated.h"


//USTRUCT()
//struct FMyInterpStruct
//{
//    GENERATED_BODY()
//
//        UPROPERTY()
//        float Brightness;
//
//    UPROPERTY()
//        float BrightnessGoal; //interping to
//
//    UPROPERTY()
//        FVector Location;
//
//    UPROPERTY()
//        FVector LocationGoal;
//
//    UPROPERTY()
//        float Alpha;
//
//
//    void InterpInternal()
//    {
//        Location = FMath::Lerp<FVector>(Location, LocationGoal, Alpha);
//        Brightness = FMath::Lerp<float>(Brightness, BrightnessGoal, Alpha);
//    }
//
//    //Brightness out is returned, FVector is returned by reference 
//    float Interp(const float& NewAlpha, FVector& Out)
//    {
//        // value received from rest of your game engine
//        Alpha = NewAlpha;
//
//        //Internal data structure management
//        InterpInternal();
//
//        //Return Values
//        Out = Location;
//        return Brightness;
//    }
//
//    FMyInterpStruct()
//    {
//        Brightness = 2;
//        BrightnessGoal = 100;
//        Alpha = 0;
//        Location = FVector::ZeroVector;
//        LocationGoal = FVector(0, 0, 200000);
//    }
//};


namespace MarchingCubeUtils
{
	//USTRUCT(BlueprintType)
	//struct FNoiseSampleParams
	//{
	//	GENERATED_BODY()

	//	UPROPERTY(BlueprintReadWrite)
	//	float FreqScale = 1.f;

	//	UPROPERTY(BlueprintReadWrite)
	//	float AmplitudeScale = 1.f;
	//};

	class WorldGridBuilder
	{
	public:

		// TODO: ctors
		WorldGridBuilder() {};
		WorldGridBuilder(const FIntVector& dimensions, float size = 1.f, float isolevel = 1.f);

		// TODO: improve encapsulation here
		void SetCells(const FIntVector& dimensions, float size = 1.f, float isolevel = 1.f);

		void SetCells(float size, float isolevel);

		void SetCell(const WorldGridCell& cell, int x, int y, int z);

		void SetDensityValues(const TArray<FNoiseSampler>& noiseSamplers, float allNoiseScale);

		// TODO: can be private with getter for triangles?
		void Polygonise(float isolevel, TArray<PolygoniseUtils::Triangle>& trianglesOut);

		const FIntVector& Dimensions() const { return _dimensions; }

		float Size() const { return _size; }

	private:

		FIntVector _dimensions;

		float _size;

		TArray<WorldGridCell> _gridCells;

		double DensityFunction(const FVector& corner, const TArray<FNoiseSampler>& noiseSamplers, float allNoiseScale);
	};
}