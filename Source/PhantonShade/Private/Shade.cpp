// Fill out your copyright notice in the Description page of Project Settings.


#include "Shade.h"

// Sets default values
AShade::AShade() : Super()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UProceduralMeshComponent* ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcShadow"));
	ProcMesh->bUseComplexAsSimpleCollision = false;

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

