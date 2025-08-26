// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "../ShadowEngine/I_ShadowMeshInterface.h"
#include "Shade.generated.h"

UCLASS()
class PHANTONSHADE_API AShade : public AActor, public II_ShadowMeshInterface
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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetProceduralMeshLocation_Implementation() override;
	FRotator GetProceduralRotation_Implementation() override;
	void UpdateShadowActorMesh_Implementation(int32 SectionID, const TArray<FVector>& VerticesArray, const TArray<int32>& TriangelsArray) override;
	void UpdateShadowActorMeshTransform_Implementation(const FTransform& ObjTransform) override;
	//void UpdateShadowActorMeshTransform_Implementation(const FTransform& ObjTransform) override;
};
