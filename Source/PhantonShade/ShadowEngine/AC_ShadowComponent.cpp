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

void UAC_ShadowComponent::ShowStat()
{
	UE_LOG(LogTemp, Warning, TEXT("=== Light Actors Positions ==="));

	for (int32 i = 0; i < LightActors.Num(); i++)
	{
		const TSoftObjectPtr<AActor>& SoftActor = LightActors[i];

		if (AActor* Actor = SoftActor.Get())
		{
			// Перевіряємо чи актор реалізує інтерфейс
			if (Actor->Implements<ULightSoursInterface>())
			{
				// Викликаємо інтерфейс через Execute
				FVector Position = ILightSoursInterface::Execute_GetLightSourPosition(Actor);
				UE_LOG(LogTemp, Warning, TEXT("Actor [%d] %s Position: %s"),
					i, *Actor->GetName(), *Position.ToString());
			}
			else
			{
				// Fallback - стандартна позиція актора
				FVector Position = Actor->GetActorLocation();
				UE_LOG(LogTemp, Warning, TEXT("Actor [%d] %s Position (fallback): %s"),
					i, *Actor->GetName(), *Position.ToString());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Actor [%d]: NULL or not loaded"), i);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("=== End Light Actors Positions ==="));
}

