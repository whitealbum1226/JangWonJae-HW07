#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class HW07_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	AMyPawn();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "CharacterComponent")
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "CharacterComponent")
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "CharacterComponent")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "CharacterComponent")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MoveSpeed = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float LookSpeed = 50.f;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Gravity = -980.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float AirControlMultiplier = 0.5f;

private:
	float CurrentDeltaTime = 0.f;
	float VerticalVelocity = 0.f;
	bool bIsGround = false;
};
