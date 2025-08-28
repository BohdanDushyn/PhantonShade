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
	//MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//MeshComponent->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void AShade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AShade::GetProceduralMeshLocation_Implementation()
{
	return MeshComponent->GetComponentLocation();
}

FRotator AShade::GetProceduralRotation_Implementation()
{
	return MeshComponent->GetComponentRotation();
}

void AShade::UpdateShadowActorMesh_Implementation(int32 SectionID, const TArray<FVector>& VerticesArray, const TArray<int32>& TriangelsArray)  
{  
   if (MeshComponent)  
   {  
       /*TArray<FVector> Normals;  
       TArray<FVector2D> UV0;  
       TArray<FColor> VertexColors;  
       TArray<FProcMeshTangent> Tangents;

       MeshComponent->CreateMeshSection(SectionID, VerticesArray, TriangelsArray, Normals, UV0, VertexColors, Tangents, true);  */

       MeshComponent->CreateMeshSection(SectionID, VerticesArray, TriangelsArray, TArray<FVector>{}, TArray<FVector2D>{}, TArray<FColor>{}, TArray<FProcMeshTangent>{}, true);   
   }  
   else  
   {  
       UE_LOG(LogTemp, Warning, TEXT("MeshComponent is not initialized!"));  
   }  
}

void AShade::UpdateShadowActorMeshTransform_Implementation(const FTransform& ObjTransform)
{
	SetActorTransform(ObjTransform);
}

/*
void AShade::UpdateShadowActorMeshTransform_Implementation(const FTransform& ObjTransform)
{
	SetActorTransform(ObjTransform);
}*/
