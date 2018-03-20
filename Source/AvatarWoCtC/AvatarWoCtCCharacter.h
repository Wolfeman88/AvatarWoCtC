// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AvatarWoCtCCharacter.generated.h"

UENUM(BlueprintType)		
enum class EBendingDisciplineType : uint8
{
	BDT_None		UMETA(DisplayName = "Non-bender"),
	BDT_Air 		UMETA(DisplayName = "Airbender"),
	BDT_Earth 		UMETA(DisplayName = "Earthbender"),
	BDT_Fire		UMETA(DisplayName = "Firebender"),
	BDT_Water		UMETA(DisplayName = "Waterbender")
};

UCLASS(config=Game)
class AAvatarWoCtCCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	bool bCanDoubleJump = false;
	bool bCanHover = false;
	bool bJumpHeld = false;
	
	float fDefaultGravityScale = 1.f;
	float fDefaultAirControl = 0.2f;

	float fDefaultBoomLength = 400.f;

public:
	AAvatarWoCtCCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "Ranged Mode")
	bool bIsRangedToggle = false;

private:

	void CheckHover();

	void CenterCamera();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bending")
	EBendingDisciplineType BendingDiscipline = EBendingDisciplineType::BDT_None;

	/** Called for forwards/backward input */
	void MoveForward(float Value);
	/** Called for side to side input */
	void MoveRight(float Value);
	/** @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate */
	void TurnAtRate(float Rate);
	void RotateYaw(float Rate);
	/** @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate */
	void LookUpAtRate(float Rate);
	void RotatePitch(float Rate);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jumping")
	float fHoverGravityScale = 0.25f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jumping")
	float fHoverAirControl = 2.f;

	void StartJump();
	void EndJump();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged")
	float fRangedBoomLength = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged")
	float fCameraShiftOffset = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged")
	bool bRangedModeActive = false;

	void StartRangedMode();
	void EndRangedMode();

};

