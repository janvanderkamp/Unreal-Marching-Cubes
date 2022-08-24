// Fill out your copyright notice in the Description page of Project Settings.


#include "MarchingCubeUtils.h"

DEFINE_LOG_CATEGORY(LogGrid);

using namespace MarchingCubeUtils;


const FVector WorldGridCell::LeftDownForwardVector		= (FVector::LeftVector + FVector::DownVector + FVector::ForwardVector) / 2.f;
const FVector WorldGridCell::RightDownForwardVector		= (FVector::RightVector + FVector::DownVector + FVector::ForwardVector) / 2.f;
const FVector WorldGridCell::RightDownBackwardVector	= (FVector::RightVector + FVector::DownVector + FVector::BackwardVector) / 2.f;
const FVector WorldGridCell::LeftDownBackwardVector		= (FVector::LeftVector	+ FVector::DownVector	+ FVector::BackwardVector)	/ 2.f;
const FVector WorldGridCell::LeftUpForwardVector		= (FVector::LeftVector + FVector::UpVector + FVector::ForwardVector) / 2.f;
const FVector WorldGridCell::RightUpForwardVector		= (FVector::RightVector + FVector::UpVector + FVector::ForwardVector) / 2.f;
const FVector WorldGridCell::RightUpBackwardVector		= (FVector::RightVector + FVector::UpVector + FVector::BackwardVector) / 2.f;
const FVector WorldGridCell::LeftUpBackwardVector		= (FVector::LeftVector	+ FVector::UpVector		+ FVector::BackwardVector)	/ 2.f;


