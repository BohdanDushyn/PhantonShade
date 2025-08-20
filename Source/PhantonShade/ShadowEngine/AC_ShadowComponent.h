// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_ShadowComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PHANTONSHADE_API UAC_ShadowComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_ShadowComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Math Calculator", meta = (DisplayName = "First Number"))
	float FirstNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Math Calculator", meta = (DisplayName = "Second Number"))
	float SecondNumber;

public:	
	UFUNCTION(BlueprintCallable, Category = "Math Calculator", meta = (DisplayName = "Set Numbers"))
	void SetNumbers(float InFirstNumber, float InSecondNumber);

	// Функція для отримання результату з параметрами (доступна в Blueprint)
	UFUNCTION(BlueprintCallable, Category = "Math Calculator", meta = (DisplayName = "Add Two Numbers"))
	float AddTwoNumbers(float A, float B);

	// Геттери для чисел
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math Calculator", meta = (DisplayName = "Get First Number"))
	float GetFirstNumber() const { return FirstNumber; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math Calculator", meta = (DisplayName = "Get Second Number"))
	float GetSecondNumber() const { return SecondNumber; }
		
};
