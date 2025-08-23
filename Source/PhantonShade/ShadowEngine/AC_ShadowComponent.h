// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProceduralMeshComponent.h"
#include "../ShadowEngine/LightSoursInterface.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "AC_ShadowComponent.generated.h"


USTRUCT(BlueprintType)
struct FLineTraceResult
{
    GENERATED_BODY()

    //UPROPERTY(BlueprintReadOnly)
    //FVector StartPointResult;

    UPROPERTY(BlueprintReadOnly)
    FVector EndPointResult;

    UPROPERTY(BlueprintReadOnly)
    bool bIsTraced;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PHANTONSHADE_API UAC_ShadowComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_ShadowComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    FTimerHandle MyTimerHandle;

	float TimerInterval = 1.0f;

public:	

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    TArray<TSoftObjectPtr<AActor>> LightActors;

    UFUNCTION(BlueprintCallable, Category = "LightingTimer")
    void StartShadowCalculate();

    UFUNCTION(BlueprintCallable, Category = "LightingTimer")
    void StartShadowCalculateWithSetTimer(float NewTimer);
    
    UFUNCTION(BlueprintCallable, Category = "LightingTimer")
    void SetTimerInterval(float NewTimerInterval);

    // Додати один актор до масиву
    UFUNCTION(BlueprintCallable, Category = "Lighting")
    void AddLightActor(AActor* Actor);

    // Встановити весь масив акторів
    UFUNCTION(BlueprintCallable, Category = "Lighting")
    void SetLightActors(const TArray<AActor*>& Actors);

    // Очистити масив
    UFUNCTION(BlueprintCallable, Category = "Lighting")
    void ClearLightActors();

    // Видалити конкретний актор з масиву
    UFUNCTION(BlueprintCallable, Category = "Lighting")
    void RemoveLightActor(AActor* Actor);

    // Отримати всі актори (завантажені)
    UFUNCTION(BlueprintCallable, Category = "Lighting")
    TArray<AActor*> GetLoadedLightActors();

    // Перевірити чи містить масив конкретний актор
    UFUNCTION(BlueprintCallable, Category = "Lighting")
    bool ContainsLightActor(AActor* Actor);
		
    UFUNCTION(BlueprintCallable, Category = "Lighting")
    int32 GetAmount();

    UFUNCTION(BlueprintCallable, Category = "ShadowFunction")
    FLineTraceResult LineTraceWithOffset(const FVector& LightStartLocation, float OffsetX, float OffsetZ, float RayMaxLength);

    UFUNCTION(BlueprintCallable, Category = "ShadowFunction")
    FVector2D MakeOffset(FVector OffsetValue, FVector LightPosition);

    UFUNCTION(BlueprintCallable, Category = "ShadowFunction")
    TArray<FVector> MakeShadowFloor(FVector Offset, FVector LightStartLocation, float RayMaxLenght, int AmountOfPieces);

    UFUNCTION(BlueprintCallable, Category = "Lighting")
    void ShowAllStat();

    void ShowOneStat(const TSoftObjectPtr<AActor>& LightActorPtr, int32 id);
};
