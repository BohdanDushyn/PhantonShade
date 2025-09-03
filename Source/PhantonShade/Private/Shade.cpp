// Fill out your copyright notice in the Description page of Project Settings.


#include "Shade.h"

// Sets default values
AShade::AShade() : Super()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcShadow"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->bUseAsyncCooking = true;
	MeshComponent->bUseComplexAsSimpleCollision = true;
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//MeshComponent->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void AShade::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShade::UpdateShadowActorMeshes(int32 SectionID, const TArray<FVector>& VerticesArray, const TArray<int32>& TriangelsArray)
{
	if (MeshComponent)
	{
		
		FFunctionGraphTask::CreateAndDispatchWhenReady([this, SectionID, VerticesArray, TriangelsArray]() {
			MeshComponent->CreateMeshSection(SectionID, VerticesArray, TriangelsArray, TArray<FVector>{}, TArray<FVector2D>{}, TArray<FColor>{}, TArray<FProcMeshTangent>{}, true);
			}, TStatId(), nullptr, ENamedThreads::GameThread); 

		//MeshComponent->CreateMeshSection(SectionID, VerticesArray, TriangelsArray, TArray<FVector>{}, TArray<FVector2D>{}, TArray<FColor>{}, TArray<FProcMeshTangent>{}, true);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MeshComponent is not initialized!"));
	}
}

void AShade::RemoveMeschSection()
{
	MeshComponent->ClearAllMeshSections();
}

TArray<AActor*> AShade::GetAllOverlapingActors()
{
	TArray<AActor*> LocalOverlappingActors;
	MeshComponent->GetOverlappingActors(LocalOverlappingActors);
	return LocalOverlappingActors;
}


FVector AShade::GetProceduralMeshLocationNotInterf()
{
	return MeshComponent->GetComponentLocation();
}

FRotator AShade::GetProceduralRotationNotInterf()
{
	return MeshComponent->GetComponentRotation();
}

void AShade::UpdateShadowActorMeshTransform(const FTransform& ObjTransform)
{
	SetActorTransform(ObjTransform);
}

