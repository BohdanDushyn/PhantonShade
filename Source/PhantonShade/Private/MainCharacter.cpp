// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"



AMainCharacter::AMainCharacter() : Super()
{
	this->CharacterLevel = 1.0f;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCameraComponent"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) 
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(this->InputMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(this->TestAction, ETriggerEvent::Triggered, this, &AMainCharacter::TestActionPressed);

		Input->BindAction(this->MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
	}
}

struct FLightProcessingTask : public FRunnable
{
	AActor* LightSource;
	int32 SectionID;
	AMainCharacter* Owner;
	FRunnableThread* Thread;

	FLightProcessingTask(AActor* InLightSource, int32 InSectionID, AMainCharacter* InOwner)
		: LightSource(InLightSource), SectionID(InSectionID), Owner(InOwner), Thread(nullptr)
	{
	}

	virtual bool Init() override { return true; }
	virtual uint32 Run() override;
	virtual void Stop() override {}
};

uint32 FLightProcessingTask::Run()
{
	FPlatformProcess::Sleep(0.01f); // Невелика затримка (опціонально)

	if (Owner && LightSource)
	{
		// Безпосередньо викликаємо функцію CreateShadow на об'єкті Owner (AMyLightManager)
		Owner->CreateShadow(LightSource, SectionID);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner is null in FLightProcessingTask!"));
	}
	return 0;
}

void AMainCharacter::ProcessLightSources()
{
	int32 NumLightSources = LightSources.Num();
	if (NumLightSources > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Processing %d light sources in separate threads."), NumLightSources);
		for (int32 i = 0; i < NumLightSources; ++i)
		{
			if (AActor* LightSource = LightSources[i])
			{
				// Створення нової задачі для потоку, передаючи 'this' як Owner
				FLightProcessingTask* Task = new FLightProcessingTask(LightSource, i, this);
				FRunnableThread* Thread = FRunnableThread::Create(Task, *FString::Printf(TEXT("LightProcessingThread_%d"), i));
				Task->Thread = Thread;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No light sources to process."));
	}
}

void AMainCharacter::ProcessSingleLightSource(AActor* LightSource, int32 SectionID)
{
	// Ця функція більше не використовується безпосередньо в потоках,
	// але може бути корисною для послідовної обробки, якщо знадобиться.
	CreateShadow(LightSource, SectionID);
}

void AMainCharacter::CreateShadow_Implementation(AActor* LightActor, int32 SectionIndex)
{
	// За замовчуванням, C++ реалізація може бути порожньою або містити базову логіку.
	// Blueprint-клас, похідний від AMyLightManager, перекриє цю функцію.
	UE_LOG(LogTemp, Warning, TEXT("C++ CreateShadow_Implementation called (should be overridden in Blueprint)."));
}

void AMainCharacter::CreateShadow(AActor* LightActor, int32 SectionIndex)
{
	// Ця частина необхідна для BlueprintNativeEvent, щоб викликати Blueprint-реалізацію.
	// Виклик _Implementation функції призведе до виклику Blueprint-перевизначення, якщо воно існує.
	CreateShadow_Implementation(LightActor, SectionIndex);
}



void AMainCharacter::TestActionPressed()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Test Action Pressed"));
}

void AMainCharacter::Move(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{

		//const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);

		//const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		//
		//InputVector = InputVector.GetSafeNormal();

		////GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(InputVector.Length()));;

		//AddMovementInput(ForwardDirection, InputVector.Length());

		

        InputVector = InputVector.GetSafeNormal();

        //FRotator SpringArmRotation = SpringArm->GetComponentRotation();
        FRotator SpringArmRotation = SpringArm->GetRelativeRotation();

        if (!InputVector.IsZero())
        {
            FVector TDVector(InputVector.X, InputVector.Y, 0.0f);
            FRotator Rotation(0, SpringArmRotation.Yaw + 90, 0);
            FVector Rotated3DVector = FRotationMatrix(Rotation).TransformVector(TDVector);
            InputVector = FVector2D(Rotated3DVector.X, Rotated3DVector.Y);

            float TargetYaw = FMath::RadiansToDegrees(FMath::Atan2(InputVector.Y, InputVector.X));

            FRotator TargetRotation = FRotator(0, TargetYaw, 0);

            /*
            FString RotationString = FString::Printf(TEXT("SpringArm Rotation: Pitch=%.2f, Yaw=%.2f, Roll=%.2f, TargetYaw=%.2f"),
                //SpringArmRotation.Pitch,
                //SpringArmRotation.Yaw,
                //SpringArmRotation.Roll,
                InputVector.X,
				InputVector.Y,
				TargetRotation.Pitch,
                SpringArmRotation.Yaw);
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, RotationString);
            */

            FRotator CurrentRotation = Controller->GetControlRotation();
            FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 50.0f);
            Controller->SetControlRotation(NewRotation);

            const FVector ForwardDirection = FRotationMatrix(NewRotation).GetUnitAxis(EAxis::X);
            const FVector RightDirection = FRotationMatrix(NewRotation).GetUnitAxis(EAxis::Y);

			AddMovementInput(ForwardDirection, InputVector.Length());
        }

        //InputVector = InputVector.GetSafeNormal();

        //if (!InputVector.IsZero())
        //{
        //    // 1. Отримуємо ротацію SpringArm (лише Yaw)
        //    FRotator SpringArmRotation = SpringArm->GetComponentRotation();
        //    float SpringArmYaw = SpringArmRotation.Yaw;

        //    // 2. Обчислюємо кут руху відносно SpringArm
        //    float InputYaw = FMath::RadiansToDegrees(FMath::Atan2(InputVector.Y, InputVector.X));
        //    float TargetYaw = SpringArmYaw;

        //    // 3. Визначаємо напрямки руху (відносно камери)
        //    const FVector ForwardDirection = FRotationMatrix(SpringArmRotation).GetUnitAxis(EAxis::X);
        //    const FVector RightDirection = FRotationMatrix(SpringArmRotation).GetUnitAxis(EAxis::Y);

        //    // 4. Рухаємо персонажа
        //    AddMovementInput(ForwardDirection, InputVector.Y);
        //    AddMovementInput(RightDirection, InputVector.X);

        //    // 5. Плавний поворот персонажа
        //    FRotator TargetRotation(0, 0, TargetYaw);
        //    FRotator CurrentRotation = GetActorRotation();
        //    FRotator NewRotation = FMath::RInterpTo(
        //        CurrentRotation,
        //        TargetRotation,
        //        GetWorld()->GetDeltaSeconds(),
        //        15.0f // Швидкість повороту
        //    );

        //    SetActorRotation(FRotator(0, NewRotation.Yaw, 0));
        //}
        
	}
}
