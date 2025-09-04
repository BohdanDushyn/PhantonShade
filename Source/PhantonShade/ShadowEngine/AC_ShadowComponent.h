// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Shade.h" 
#include "LightActor.h"
#include "Components/ActorComponent.h"
#include "ProceduralMeshComponent.h"
#include "../ShadowEngine/LightSoursInterface.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
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

USTRUCT(BlueprintType)
struct FOffsetResultVector
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    float X;

    UPROPERTY(BlueprintReadOnly)
    float Z;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PHANTONSHADE_API UAC_ShadowComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_ShadowComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    TArray<TSoftObjectPtr<AActor>> LightActors;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shadow Actor Settings", meta = (AllowedClasses = "Actor"))
    TSubclassOf<AActor> ShadowActorClass;

protected:
    UWorld* WorldPtr = GetWorld();

    UClass* LightActorBlueprintClass;

	virtual void BeginPlay() override;

	FVector OwnerLocation;

	FVector OwnerForwardVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimerFunction")
    bool bTimerPaused = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadowFunction")
    bool IsStarted = false;

    FTimerHandle MyTimerHandle;

    FVector MeshLocationVector;

    FRotator MeshRotator;

	float TimerInterval = 1.0f;

    AActor* ParentActor;

    AShade* CastedShadeActor;

    float OffsetFromPlane = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadowFunction")
    int AmountOfPieces = 2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadowFunction")
    TArray<FVector> MapOfShadow;

public:	 
	void SpawnShadowActor();

    UFUNCTION(BlueprintCallable, Category = "Overlaping")
    TArray<AActor*> GetShadowOverlapingActors(){ return CastedShadeActor->GetAllOverlapingActors();}

    UFUNCTION(BlueprintCallable, Category = "Overlaping")
    void SetParentActor();

    UFUNCTION(BlueprintCallable, Category = "LightingTimer")
    void SetTimerInterval(float NewTimerInterval);

    UFUNCTION(BlueprintCallable, Category = "LightingTimer")
    void PauseTimer();

    UFUNCTION(BlueprintCallable, Category = "LightingTimer")
	void ResumeTimer();

    UFUNCTION(BlueprintCallable, Category = "Lighting")
    void AddLightActor(AActor* Actor);

    UFUNCTION(BlueprintCallable, Category = "Lighting")
    void SetLightActors(const TArray<AActor*>& Actors);

    UFUNCTION(BlueprintCallable, Category = "Lighting")
    void ClearLightActors();

    UFUNCTION(BlueprintCallable, Category = "Lighting")
    void RemoveLightActor(AActor* Actor);

    UFUNCTION(BlueprintCallable, Category = "Lighting")
    TArray<AActor*> GetLoadedLightActors();

    UFUNCTION(BlueprintCallable, Category = "Lighting")
    bool ContainsLightActor(AActor* Actor);
		
    UFUNCTION(BlueprintCallable, Category = "Lighting")
    int32 GetLightSoursAmount();

    UFUNCTION(BlueprintCallable, Category = "ShadowFunction")
    void StartShadowCalculateWithParams(float TimerDelay, TArray<FVector> NewMapOfShadow, const TArray<AActor*>& NewLightActors, int AmountOfFloorPieces);

    UFUNCTION(BlueprintCallable, Category = "ShadowFunction")
    void StartShadowCalculate();

    UFUNCTION(BlueprintCallable, Category = "ShadowFunction")
    void StartShadowCalculateWithSetTimer(float NewTimer);

    UFUNCTION(BlueprintCallable, Category = "ShadowFunction")
    void SetShadeActor(AActor* NewShadeActor);

    UFUNCTION(BlueprintCallable, Category = "ShadowFunction")
	void SetAmountOfPieces(int NewAmount) { AmountOfPieces = NewAmount; }

    UFUNCTION(BlueprintCallable, Category = "ShadowFunction")
    int GetAmountOfPieces() { return AmountOfPieces; }

    UFUNCTION(BlueprintCallable, Category = "ShadowFunction")
	void SetMapOfShadow(const TArray<FVector>& NewMapOfShadow) { MapOfShadow = NewMapOfShadow; }

    UFUNCTION(BlueprintCallable, Category = "ShadowFunction")
    FLineTraceResult LineTraceWithOffset(const FVector& LightStartLocation, FOffsetResultVector Offset, float RayMaxLength);

    UFUNCTION(BlueprintCallable, Category = "ShadowFunction")
    FOffsetResultVector MakeOffset(FVector OffsetValue, FVector LightPosition);

    UFUNCTION(BlueprintCallable, Category = "ShadowFunction")
    TArray<FVector> MakeShadowFloor(FVector OffsetValue, FVector LightStartLocation, float RayMaxLenght);

    UFUNCTION(BlueprintCallable, Category = "Lighting")
    void CreateShadow();

    void CreateOneShadow(TSoftObjectPtr<AActor> LightActor, int32 id);

    //void SetLightActorsFromOverlapping();
};
