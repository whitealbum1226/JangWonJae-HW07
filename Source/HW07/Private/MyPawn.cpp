#include "MyPawn.h"
#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMyPawn::AMyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->SetSimulatePhysics(false);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComp->SetCollisionProfileName(TEXT("Pawn"));

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);
	SkeletalMeshComp->SetSimulatePhysics(false);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	SpringArmComp->bUsePawnControlRotation = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;
}

void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Move
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Look
				);
			}
		}
	}
}

void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentDeltaTime = DeltaTime;

	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, -100.0f);
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bIsGround = GetWorld()->LineTraceSingleByChannel(
		HitResult, StartLocation, EndLocation, ECC_Visibility, Params);

	if (bIsGround)
	{
		VerticalVelocity = 0.0f;
	}
	else
	{
		VerticalVelocity += Gravity * DeltaTime;
	}
}

void AMyPawn::Move(const FInputActionValue& value)
{
	FVector Input = value.Get<FVector>();

	if (bIsGround)
	{
		float CurrentSpeed = MoveSpeed* AirControlMultiplier;

		AddActorWorldOffset(GetActorForwardVector() * Input.X * CurrentSpeed * CurrentDeltaTime, true);
		AddActorWorldOffset(GetActorRightVector() * Input.Y * CurrentSpeed * CurrentDeltaTime, true);
		AddActorWorldOffset(GetActorUpVector() * Input.Z * CurrentSpeed * CurrentDeltaTime, true);
	}

	AddActorWorldOffset(GetActorForwardVector() * Input.X * MoveSpeed * CurrentDeltaTime, true);
	AddActorWorldOffset(GetActorRightVector() * Input.Y * MoveSpeed * CurrentDeltaTime, true);
	AddActorWorldOffset(GetActorUpVector() * Input.Z * MoveSpeed * CurrentDeltaTime, true);
}

void AMyPawn::Look(const FInputActionValue& value)
{
	FVector Input = value.Get<FVector>();
	
	AddActorLocalRotation(FRotator(
		Input.Y * LookSpeed * CurrentDeltaTime,
		Input.X * LookSpeed * CurrentDeltaTime, 
		Input.Z * LookSpeed * CurrentDeltaTime));
}

