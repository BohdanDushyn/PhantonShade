// Fill out your copyright notice in the Description page of Project Settings.

#include "AC_ShadowComponent.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UAC_ShadowComponent::UAC_ShadowComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UAC_ShadowComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UAC_ShadowComponent::StartShadowCalculate()
{
	GetWorld()->GetTimerManager().SetTimer(
		MyTimerHandle,
		this,
		&UAC_ShadowComponent::ShowAllStat,
		TimerInterval,
		true,
		FMath::FRand()
	);
}

void UAC_ShadowComponent::StartShadowCalculateWithSetTimer(float NewTimer)
{
	SetTimerInterval(NewTimer);
	GetWorld()->GetTimerManager().SetTimer(
		MyTimerHandle,
		this,
		&UAC_ShadowComponent::ShowAllStat,
		NewTimer,
		true,
		0.0f
	);
}

void UAC_ShadowComponent::SetTimerInterval(float NewTimerInterval)
{
	TimerInterval = NewTimerInterval;
	if (GetWorld()->GetTimerManager().IsTimerActive(MyTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(
			MyTimerHandle,
			this,
			&UAC_ShadowComponent::ShowAllStat,
			TimerInterval,
			true,
			0.0f
		);
	}

}

void UAC_ShadowComponent::AddLightActor(AActor* Actor)
{
	if (Actor)
	{
		TSoftObjectPtr<AActor> SoftActor(Actor);
		LightActors.AddUnique(SoftActor);
	}
}

void UAC_ShadowComponent::SetLightActors(const TArray<AActor*>& Actors)
{
	LightActors.Empty();

	for (AActor* Actor : Actors)
	{
		if (Actor)
		{
			TSoftObjectPtr<AActor> SoftActor(Actor);
			LightActors.Add(SoftActor);
		}
	}
}

void UAC_ShadowComponent::ClearLightActors()
{
	LightActors.Empty();
}

void UAC_ShadowComponent::RemoveLightActor(AActor* Actor)
{
	if (Actor)
	{
		TSoftObjectPtr<AActor> SoftActor(Actor);
		LightActors.Remove(SoftActor);
	}
}

TArray<AActor*> UAC_ShadowComponent::GetLoadedLightActors()
{
	TArray<AActor*> LoadedActors;

	for (const TSoftObjectPtr<AActor>& SoftActor : LightActors)
	{
		if (AActor* Actor = SoftActor.Get())
		{
			LoadedActors.Add(Actor);
		}
	}

	return LoadedActors;
}

bool UAC_ShadowComponent::ContainsLightActor(AActor* Actor)
{
	if (!Actor)
	{
		return false;
	}

	TSoftObjectPtr<AActor> SoftActor(Actor);
	return LightActors.Contains(SoftActor);
}

int32 UAC_ShadowComponent::GetAmount()
{
	return LightActors.Num();
}

FLineTraceResult UAC_ShadowComponent::LineTraceWithOffset(const FVector& LightStartLocation, float OffsetX, float OffsetZ, float RayMaxLength)
{
	FLineTraceResult Result;
	Result.bIsTraced = false;
	FVector StartPoint = LightStartLocation;
	FVector CalculatedXOffset = (GetOwner()->GetActorLocation() - LightStartLocation).GetSafeNormal().Cross(FVector(0, 0, 1)) * OffsetX;
	CalculatedXOffset += GetOwner()->GetActorLocation() - LightStartLocation;
	CalculatedXOffset += FVector(0, 0, OffsetZ);
	FVector EndPoint = LightStartLocation + (CalculatedXOffset * RayMaxLength);

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;

	bool bHit = GetWorld()->LineTraceMultiByChannel(
		HitResults,
		StartPoint,
		EndPoint,
		ECollisionChannel::ECC_EngineTraceChannel1,
		Params
	);

	FColor LineColor = bHit ? FColor::Red : FColor::Green;
	DrawDebugLine(
		GetWorld(),
		StartPoint,
		EndPoint,
		LineColor,
		false,      // persistent lines
		0.1f,       // lifetime (seconds)
		0,          // depth priority
		2.0f        // thickness
	);

	int8 LastHit = HitResults.Num() - 1;
	if (HitResults[0].GetComponent() == GetOwner()->FindComponentByClass<UCapsuleComponent>() && HitResults[LastHit].GetActor() != GetOwner()) {
		//Result.StartPoint = HitResults[0].ImpactPoint;
		Result.EndPointResult = HitResults[LastHit].ImpactPoint;
		Result.bIsTraced = true;
	}

	return FLineTraceResult();
}

FVector2D UAC_ShadowComponent::MakeOffset(FVector OffsetValue, FVector LightPosition)
{
	double Dot = FVector::DotProduct((LightPosition - GetOwner()->GetActorLocation()).GetSafeNormal(), GetOwner()->GetActorForwardVector().GetSafeNormal());
	return FVector2D((OffsetValue * FVector(FMath::Abs(Dot), 1 - FMath::Abs(Dot), 0)).Length(), OffsetValue.Z);
}

TArray<FVector> UAC_ShadowComponent::MakeShadowFloor(FVector Offset, FVector LightStartLocation, float RayMaxLenght, int AmountOfPieces)
{

	return TArray<FVector>();
}

void UAC_ShadowComponent::ShowAllStat()
{
	//UE_LOG(LogTemp, Warning, TEXT("=== Light Actors Positions ==="));

	for (int32 i = 0; i < LightActors.Num(); i++)
	{
		FFunctionGraphTask::CreateAndDispatchWhenReady([this, i] {
			ShowOneStat(LightActors[i], i);
			}, TStatId(), nullptr, ENamedThreads::AnyBackgroundHiPriTask);
	}
	//UE_LOG(LogTemp, Warning, TEXT("=== End Light Actors Positions ==="));
}

void UAC_ShadowComponent::ShowOneStat(const TSoftObjectPtr<AActor>& LightActorPtr, int32 id)
{
	const TSoftObjectPtr<AActor>& SoftActor = LightActorPtr;

	if (AActor* Actor = SoftActor.Get())
	{
		// Перевіряємо чи актор реалізує інтерфейс
		if (Actor->Implements<ULightSoursInterface>())
		{
			// Викликаємо інтерфейс через Execute
			FVector Position = ILightSoursInterface::Execute_GetLightSourPosition(Actor);
			//UE_LOG(LogTemp, Warning, TEXT("Actor [%d] %s Position: %s"), id, *Actor->GetName(), *Position.ToString());
		}
		else
		{
			// Fallback - стандартна позиція актора
			FVector Position = Actor->GetActorLocation();
			//UE_LOG(LogTemp, Warning, TEXT("Actor [%d] %s Position (fallback): %s"), id, *Actor->GetName(), *Position.ToString());
		}
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("Actor [%d]: NULL or not loaded"), id);
	}
}

