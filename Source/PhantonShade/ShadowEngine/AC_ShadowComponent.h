// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProceduralMeshComponent.h"
#include "AC_ShadowComponent.generated.h"


USTRUCT(BlueprintType)
struct FLightSourceParams
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float SourceRadius = 0.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float AttenuationRadius = 0.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FVector Location = FVector::ZeroVector;

    FLightSourceParams()
    {
        SourceRadius = 0.0f;
        AttenuationRadius = 0.0f;
        Location = FVector::ZeroVector;
    }
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

public:	

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting")
    TArray<TSoftObjectPtr<AActor>> LightActors;

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

};
