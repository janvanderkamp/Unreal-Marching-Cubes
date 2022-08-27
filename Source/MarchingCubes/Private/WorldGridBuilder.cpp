#include "WorldGridBuilder.h"

using namespace MarchingCubeUtils;
using namespace PolygoniseUtils;

WorldGridBuilder::WorldGridBuilder(const FIntVector& dimensions, float size, float isolevel) :
	_dimensions(dimensions)
{
	SetCells(size, isolevel);
}

void WorldGridBuilder::SetCells(const FIntVector& dimensions, float size, float isolevel)
{
	_dimensions = dimensions;
	SetCells(size, isolevel);
}

void WorldGridBuilder::SetCells(float size, float isolevel)
{
	_size = size;
	double start = FPlatformTime::Seconds();
	_gridCells.SetNumUninitialized(_dimensions.X * _dimensions.Y * _dimensions.Z);

	// Keep grid centered
	FVector halfDimensions(_dimensions / 2.f);

	for (size_t x = 0; x < _dimensions.X; ++x)
	{
		for (size_t y = 0; y < _dimensions.Y; ++y)
		{
			for (size_t z = 0; z < _dimensions.Z; ++z)
			{
				FVector pos = (FVector(x, y, z) - halfDimensions) * _size;
				WorldGridCell cell(pos, _size, isolevel);

				// TODO: avoid duplicate corners
				SetCell(cell, x, y, z);
				//cell.LogCell();
			}
		}
	}

	double end = FPlatformTime::Seconds();
	UE_LOG(LogTemp, Log, TEXT("WorldGridBuilder::Set - dimensions: (%s), took %f seconds"), *_dimensions.ToString(), end - start)
}

void WorldGridBuilder::SetDensityValues(const TArray<FNoiseSampler>& noiseSamplers, float allNoiseScale)
{
	int numCellsPlane = _dimensions.X * _dimensions.Y;
	for (size_t x = 0; x < _dimensions.X; ++x)
	{
		if (x % (_dimensions.X / 10) == 0)
		{
			UE_LOG(LogGrid, Log, TEXT("WorldGridBuilder::SetDensityValues - progress: %d / 10"), x / 10 + 1);
		}
		for (size_t y = 0; y < _dimensions.Y; ++y)
		{
			for (size_t z = 0; z < _dimensions.Z; ++z)
			{
				for (size_t i = 0; i < 8; i++)
				{
					WorldGridCell& worldGridCell = _gridCells[x + _dimensions.X * y + numCellsPlane * z];
					worldGridCell.GridCell.values[i] = DensityFunction(worldGridCell.GridCell.points[i], noiseSamplers, allNoiseScale);
				}
			}
		}
	}
}

void WorldGridBuilder::SetCell(const WorldGridCell& cell, int x, int y, int z)
{
	int numCellsPlane = _dimensions.X * _dimensions.Y;
	_gridCells[x + _dimensions.X * y + numCellsPlane * z] = cell;
}

void WorldGridBuilder::Polygonise(float isolevel, TArray<Triangle>& trianglesOut)
{
	double start = FPlatformTime::Seconds();

	int numTriangles = 0;
	for (auto& cellIter : _gridCells)
	{
		numTriangles += PolygoniseUtils::Polygonise(cellIter.GridCell, isolevel, trianglesOut);
	}

	double end = FPlatformTime::Seconds();
	UE_LOG(LogGrid, Log, TEXT("WorldGridBuilder::Polygonise - numTriangles generated: %d, took %f seconds"), numTriangles, end - start);
}

double WorldGridBuilder::DensityFunction(const FVector& corner, const TArray<FNoiseSampler>& noiseSamplers, float allNoiseScale)
{
	return DensityFunctionTest(corner, noiseSamplers, allNoiseScale);
}

double WorldGridBuilder::DensityFunctionTest(const FVector& corner, const TArray<FNoiseSampler>& noiseSamplers, float allNoiseScale)
{
	double density = 0.f;

	FVector sample(corner.X / _dimensions.X, corner.Y / _dimensions.Y, corner.Z / _dimensions.Z);
	FVector toCenter = FVector(0.5f) - sample;

	density += toCenter.Length() * allNoiseScale;

	return density;
}

double WorldGridBuilder::DensityFunctionNoise(const FVector& corner, const TArray<FNoiseSampler>& noiseSamplers, float allNoiseScale)
{
	// Start by placing a 'floor' at Z == 0, which will be across the center of the grid area
	double density = corner.Z;


	return density;
}