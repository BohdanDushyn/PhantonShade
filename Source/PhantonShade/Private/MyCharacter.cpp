// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	this->Health = 100.0f;
	this->bIsDead = false;
}


float AMyCharacter::TakeDamage(float DamageAmount)
{
	this->Health -= DamageAmount;
	IsDead();
	return this->Health;
}

float AMyCharacter::GetHealth()
{
	return this->Health;
}

void AMyCharacter::SetHealth(float NewHealth)
{
	this->Health = NewHealth;
}

float AMyCharacter::Heal(float HealAmount)
{
	this->Health += HealAmount;
	return this->Health;
}

bool AMyCharacter::IsDead()
{
	if (this->Health <= 0)
	{
		bIsDead = true;

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	return bIsDead;
}
