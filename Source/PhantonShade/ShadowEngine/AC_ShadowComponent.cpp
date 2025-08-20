// Fill out your copyright notice in the Description page of Project Settings.

#include "AC_ShadowComponent.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UAC_ShadowComponent::UAC_ShadowComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	FirstNumber = 0.0f;
	SecondNumber = 0.0f;
}


// Called when the game starts
void UAC_ShadowComponent::BeginPlay()
{
	Super::BeginPlay();

	// Виведення початкових значень в лог
	UE_LOG(LogTemp, Warning, TEXT("MathCalculatorComponent initialized with values: %f + %f"), FirstNumber, SecondNumber);
	
}

void UAC_ShadowComponent::SetNumbers(float InFirstNumber, float InSecondNumber)
{
	FirstNumber = InFirstNumber;
	SecondNumber = InSecondNumber;

	UE_LOG(LogTemp, Warning, TEXT("Numbers set to: %f and %f"), FirstNumber, SecondNumber);
}

float UAC_ShadowComponent::AddTwoNumbers(float A, float B)
{
	float Result = A + B;

	UE_LOG(LogTemp, Warning, TEXT("Adding two numbers: %f + %f = %f"), A, B, Result);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue,
			FString::Printf(TEXT("Direct calculation: %.2f + %.2f = %.2f"), A, B, Result));
	}

	return Result;
}
