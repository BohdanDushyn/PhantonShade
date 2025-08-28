// Fill out your copyright notice in the Description page of Project Settings.

#include "AC_ShadowComponent.h"

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
	if (!IsValid(this) || !GetOwner() || !GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid component state in StartShadowCalculate"));
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(
		MyTimerHandle,
		this,
		&UAC_ShadowComponent::CreateShadow,
		TimerInterval,
		true,
		FMath::FRand()
	);
}

void UAC_ShadowComponent::StartShadowCalculateWithSetTimer(float NewTimer)
{
	//UE_LOG(LogTemp, Warning, TEXT("StartShadowCalculateWithSetTimer"));
	SetTimerInterval(NewTimer);
	GetWorld()->GetTimerManager().SetTimer(
		MyTimerHandle,
		this,
		&UAC_ShadowComponent::CreateShadow,
		TimerInterval,
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
			&UAC_ShadowComponent::CreateShadow,
			TimerInterval,
			true,
			0.0f
		);
	}

}

void UAC_ShadowComponent::PauseTimer()
{
	if (MyTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().PauseTimer(MyTimerHandle);
		bTimerPaused = true;
	}
}

void UAC_ShadowComponent::ResumeTimer()
{
	if (MyTimerHandle.IsValid() && bTimerPaused)
	{
		GetWorld()->GetTimerManager().UnPauseTimer(MyTimerHandle);
		bTimerPaused = false;
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

int32 UAC_ShadowComponent::GetLightSoursAmount()
{
	return LightActors.Num();
}

void UAC_ShadowComponent::StartShadowCalculateWithParams(float TimerDelay, AActor* NewShadeActor, TArray<FVector> NewMapOfShadow, const TArray<AActor*>& NewLightActors, int AmountOfFloorPieces)
{
	//UE_LOG(LogTemp, Warning, TEXT("StartShadowCalculateWithParams"));
	SetMapOfShadow(NewMapOfShadow);
	SetAmountOfPieces(AmountOfFloorPieces);
	SetShadeActor(NewShadeActor);
	SetLightActors(NewLightActors);
	StartShadowCalculateWithSetTimer(TimerDelay);
}

FLineTraceResult UAC_ShadowComponent::LineTraceWithOffset(const FVector& LightStartLocation, FOffsetResultVector Offset, float RayMaxLength)
{
	FLineTraceResult Result;
	Result.bIsTraced = false;
	FVector CalculatedXOffset = (OwnerLocation - LightStartLocation).GetSafeNormal().Cross(FVector(0, 0, 1)) * Offset.X;
	CalculatedXOffset += OwnerLocation - LightStartLocation;
	CalculatedXOffset += FVector(0, 0, Offset.Z);
	FVector EndPoint = LightStartLocation + (CalculatedXOffset.GetSafeNormal() * RayMaxLength);
	
	TArray<FHitResult> HitResults;
	
	auto TraceFunction = [this](const FVector& FuncStartPoint, const FVector& FuncEndPoint) {
		TArray<FHitResult> FuncHitResults;
		FCollisionQueryParams Params;
		if (!GetWorld())
		{
			return FuncHitResults;
		}
		bool bHit = GetWorld()->LineTraceMultiByChannel(
			FuncHitResults,
			FuncStartPoint,
			FuncEndPoint,
			ECollisionChannel::ECC_GameTraceChannel2,
			Params
		);

		// Завжди малюємо повну лінію
		/*
		FColor LineColor = bHit ? FColor::Red : FColor::Green;
		DrawDebugLine(
			GetWorld(),
			FuncStartPoint,
			FuncEndPoint,
			LineColor,
			false,      // persistent lines
			0.1f,       // lifetime (seconds)
			0,          // depth priority
			1.0f        // thickness
		);*/
		
		/*
		// Малюємо точки влучання для всіх перетинів
		if (bHit)
		{
			for (const FHitResult& HitResult : FuncHitResults)
			{
				// Малюємо точку влучання
				DrawDebugSphere(
					GetWorld(),
					HitResult.Location,
					5.0f,       // radius
					12,         // segments
					FColor::Yellow,
					false,      // persistent
					0.1f,       // lifetime
					0,          // depth priority
					1.0f        // thickness
				);
				
				
				// Опціонально: малюємо нормаль поверхні
				//DrawDebugDirectionalArrow(
				//	GetWorld(),
				//	HitResult.Location,
				//	HitResult.Location + HitResult.Normal * 30.0f,
				//	8.0f,       // arrow size
				//	FColor::Blue,
				//	false,      // persistent
				//	0.1f,       // lifetime
				//	0,          // depth priority
				//	1.5f        // thickness
				//);
				
			}
		}
		*/

		return FuncHitResults;
		};
	
	TSharedPtr<TArray<FHitResult>> SharedResults = MakeShared<TArray<FHitResult>>();

	FFunctionGraphTask::CreateAndDispatchWhenReady([SharedResults, &TraceFunction, LightStartLocation, EndPoint]() {
		*SharedResults = TraceFunction(LightStartLocation, EndPoint);
		}, TStatId(), nullptr, ENamedThreads::GameThread)->Wait();

	HitResults = *SharedResults;

	//UE_LOG(LogTemp, Warning, TEXT("HitResults %d %d"), Offset, HitResults.Num());
	int8 LastHit = HitResults.Num() - 1;
	if (HitResults.Num() >= 2)
	{
		//UE_LOG(LogTemp, Warning, TEXT("HitResults %s"), *HitResults[0].GetComponent()->GetName());
		if (HitResults[0].GetComponent() == GetOwner()->FindComponentByClass<UCapsuleComponent>() && HitResults[LastHit].GetActor() != GetOwner()) {
			//UE_LOG(LogTemp, Warning, TEXT("HitResults %s"), *HitResults[LastHit].GetComponent()->GetName());
			//Result.StartPoint = HitResults[0].ImpactPoint;
			Result.EndPointResult = HitResults[LastHit].ImpactPoint;
			Result.bIsTraced = true;
		}
	}
	
	return Result;
}

FOffsetResultVector UAC_ShadowComponent::MakeOffset(FVector OffsetValue, FVector LightPosition)
{
	double Dot = FVector::DotProduct((LightPosition - OwnerLocation).GetSafeNormal(), OwnerForwardVector.GetSafeNormal());
	return FOffsetResultVector((OffsetValue * FVector(FMath::Abs(Dot), 1 - FMath::Abs(Dot), 0)).Length(), OffsetValue.Z);
}


TArray<FVector> UAC_ShadowComponent::MakeShadowFloor(FVector OffsetValue, FVector LightStartLocation, float RayMaxLenght)
{
	
	TArray<FVector> ShadowPointMap;

	FOffsetResultVector Offset = MakeOffset(OffsetValue, LightStartLocation);
	int8 index = 0;
	if (AmountOfPieces < 2) {
		AmountOfPieces = 2;
	}
	
	
	for (int8 i = 0; i < AmountOfPieces; i++)
	{
		FLineTraceResult Result = LineTraceWithOffset(LightStartLocation, FOffsetResultVector(Offset.X - ((Offset.X / 0.5 / AmountOfPieces) * i), Offset.Z), RayMaxLenght);
		if (Result.bIsTraced) {
			ShadowPointMap.Add(Result.EndPointResult);
			index = i;
			break;
		}
	}
	
	
	if (ShadowPointMap.Num() == 1)
	{
		for (int8 i = AmountOfPieces; i > index; i--)
		{
			//FLineTraceResult Result = LineTraceWithOffset(LightStartLocation, FOffsetResultVector(Offset.X - ((Offset.X / 0.5 / AmountOfPieces - 1) * i - AmountOfPieces - 1), Offset.Z), RayMaxLenght);
			FLineTraceResult Result = LineTraceWithOffset(LightStartLocation, FOffsetResultVector(Offset.X - ((Offset.X / 0.5 / AmountOfPieces) * i), Offset.Z), RayMaxLenght);
			if (Result.bIsTraced) {
				ShadowPointMap.Add(Result.EndPointResult);
				return ShadowPointMap;
			}
		}
	}
	
	return TArray<FVector>{};
}

void UAC_ShadowComponent::CreateShadow()
{
	if (LightActors.Num() == 0) return;

	TArray<TSoftObjectPtr<AActor>> LightActorsCopy = LightActors;

	OwnerLocation = GetOwner()->GetActorLocation();
	OwnerForwardVector = GetOwner()->GetActorForwardVector();

	FGraphEventArray Tasks;
	Tasks.Reserve(LightActors.Num());

	if (!ShadeActor || !IsValid(ShadeActor))
	{
		UE_LOG(LogTemp, Error, TEXT("ShadeActor is not valid in CreateShadow"));
		return;
	}

	II_ShadowMeshInterface::Execute_UpdateShadowActorMeshTransform(ShadeActor, GetOwner()->GetActorTransform());

	MeshLocationVector = II_ShadowMeshInterface::Execute_GetProceduralMeshLocation(ShadeActor);
	MeshRotator = II_ShadowMeshInterface::Execute_GetProceduralRotation(ShadeActor) * -1;

	//MeshLocationVector = GetOwner()->GetActorLocation();
	//MeshRotator = GetOwner()->GetActorRotation();

	TWeakObjectPtr<UAC_ShadowComponent> WeakThis(this);

	
	for (int32 i = 0; i < LightActorsCopy.Num(); i++)
	{

		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
			[WeakThis, LightActorsCopy, i]()
			{
				if (UAC_ShadowComponent* ValidComponent = WeakThis.Get())
				{
					if (IsValid(ValidComponent))
					{
						ValidComponent->CreateOneShadow(LightActorsCopy[i], i);
					}
				}
			},
			TStatId(),
			nullptr,
			ENamedThreads::AnyBackgroundHiPriTask
		);
		Tasks.Add(Task);
	}

	FTaskGraphInterface::Get().WaitUntilTasksComplete(Tasks);

	/*
	for (int32 i = 0; i < LightActorsCopy.Num(); i++)
	{
		CreateOneShadow(LightActorsCopy[i], i);
	}*/
	
}

void UAC_ShadowComponent::CreateOneShadow(TSoftObjectPtr<AActor> LightActor, int32 id)
{
	//UE_LOG(LogTemp, Warning, TEXT("OK!"));

	bool IsPreviousFloorEnebel = false;
	TArray<FVector> VerticesArray;
	TArray<int> TriangelsArray;
	for (const FVector Offset : MapOfShadow)
	{
		if (!LightActor.IsValid() || !IsValid(LightActor.Get()))
		{
			UE_LOG(LogTemp, Warning, TEXT("LightActor is not valid in CreateOneShadow"));
			return;
		}

		AActor* Actor = LightActor.Get();
		if (!Actor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to get LightActor"));
			return;
		}

		TArray<FVector> ShadowFloor = MakeShadowFloor(Offset, ILightSoursInterface::Execute_GetLightSourPosition(Actor), ILightSoursInterface::Execute_GetLightSourAttenuationRadius(LightActor.Get()));
		if (ShadowFloor.Num() != 0)
		{
			for (const FVector FloorPoints : ShadowFloor) {
				FVector LocationVector = MeshRotator.RotateVector(FloorPoints - MeshLocationVector);
				VerticesArray.Add(LocationVector.GetSafeNormal() * (LocationVector.Length() - OffsetFromPlane));
			}
			if (IsPreviousFloorEnebel)
			{
				for (int i = VerticesArray.Num() - 4; i <= VerticesArray.Num() - 3; i++)
				{
					for (int j = i; j <= i+2; j++)
					{
						//UE_LOG(LogTemp, Warning, TEXT("j: %d"), j);
						TriangelsArray.Add(j);
					}
				}
			}
			else {
				IsPreviousFloorEnebel = true;
			}
		}
		else {
			IsPreviousFloorEnebel = false;
		}
		
		
		
	}
	if (VerticesArray.Num() == 0 || TriangelsArray.Num() == 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("VerticesArray or TriangelsArray is empty!"));
		return;
	}
	else {
		//UE_LOG(LogTemp, Warning, TEXT("VerticesArray or TriangelsArray is NOT empty!"));

		FFunctionGraphTask::CreateAndDispatchWhenReady([this, id, VerticesArray, TriangelsArray]() {
			II_ShadowMeshInterface::Execute_UpdateShadowActorMesh(ShadeActor, id, VerticesArray, TriangelsArray);
			}, TStatId(), nullptr, ENamedThreads::GameThread);
	}

	/*
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
	*/
}

