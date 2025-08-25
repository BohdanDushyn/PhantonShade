// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_ShadowMeshInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UI_ShadowMeshInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PHANTONSHADE_API II_ShadowMeshInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetProceduralMeshLocation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FRotator GetProceduralRotation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateShadowActorMesh(int32 SectionID, const TArray<FVector>& VerticesArray, const TArray<int32>& TriangelsArray);

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	//void UpdateShadowActorMeshTransform(const FTransform& ObjTransform);
};