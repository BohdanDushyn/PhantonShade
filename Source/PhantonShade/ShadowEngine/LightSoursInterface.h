// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LightSoursInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULightSoursInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PHANTONSHADE_API ILightSoursInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetLightSourPosition();
};
