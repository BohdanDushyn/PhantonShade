// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Shade.generated.h"

UCLASS()
class PHANTONSHADE_API AShade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShade();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UProceduralMeshComponent* MeshComponent;

	// Додаємо Root компонент для правильної ієрархії
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* DefaultSceneRoot;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void UpdateShadowActorMeshes(int32 SectionID, const TArray<FVector>& VerticesArray, const TArray<int32>& TriangelsArray);
	void RemoveMeschSection();

	UFUNCTION(BlueprintCallable, Category = "Overlaping")
	TArray<AActor*> GetAllOverlapingActors();
	
	FVector GetProceduralMeshLocationNotInterf();
	FRotator GetProceduralRotationNotInterf();

	void UpdateShadowActorMeshTransform(const FTransform& ObjTransform);

	int32 GetMeshNumSections() { return MeshComponent->GetNumSections(); }
	
};
