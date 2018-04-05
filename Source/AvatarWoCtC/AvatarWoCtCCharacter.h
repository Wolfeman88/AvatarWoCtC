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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	class UAttackComponent* AttackComp;

	bool bCanDoubleJump = false;
	bool bCanHover = false;
	bool bJumpHeld = false;
	
	float fDefaultGravityScale = 1.f;
	float fDefaultAirControl = 0.2f;

	float fDefaultBoomLength = 400.f;

	float fDefaultMoveSpeed = 600.f;

	FVector JumpDirection = FVector::ZeroVector;
	float fDefaultJumpZVelocity = 600.f;
	bool bJumpingForward = false;

	FTimerHandle RollTimerHandle;
	float fDefaultBrakingFrictionFactor = 2.f;

public:
	AAvatarWoCtCCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

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

	void ChangeSpeedWhileActivatingAbility(float SpeedFactor);

private:

	void CheckHover();

	void CenterCamera();

	void LaunchForward();

	bool CheckRollConditions();
	void ActivateRoll();
	void RollComplete();
	void RollCooldownComplete();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jumping|Hover")
	float HoverGravityScale = 0.25f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jumping|Hover")
	float HoverAirControl = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jumping|Guard")
	float GuardJumpVelocityFactor = 2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jumping|Guard")
	float GuardAirControlFactor = 0.4f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jumping|Ranged")
	float RangedJumpVelocityFactor = 1.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jumping|Ranged")
	float RangedForwardVelocityFactor = 2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jumping|Ranged")
	float RangedAirControlFactor = 0.4f;

	void StartJump();
	void EndJump();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangedMode")
	bool bRangedModeActive = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "RangedMode")
	bool bIsRangedToggle = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangedMode")
	float RangedBoomLength = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangedMode")
	float CameraShiftOffset = 100.f;

	void StartRangedMode();
	void EndRangedMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GuardMode")
	bool bGuardModeActive = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "GuardMode")
	bool bIsGuardToggle = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "GuardMode")
	float GuardModeSpeedFactor = 0.75f;

	void StartGuardMode();
	void EndGuardMode();

	void RequestLightAttack();
	void RequestHeavyAttack();
	void RequestStunAttack();

	UPROPERTY(BlueprintReadOnly, Category = "Roll")
	bool bRollActive = false;
	UPROPERTY(BlueprintReadOnly, Category = "Roll")
	bool bIsRolling = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Roll")
	float RollingSpeedFactor = 3.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Roll")
	float RollingSpeedDistance = 2.f;

	void StartRoll();
	void EndRoll();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Roll")
	int32 MaxRollCount = 2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Roll")
	float RollCooldown = 5.f;
	UPROPERTY(BlueprintReadOnly, Category = "Roll")
	TArray<FTimerHandle> RollCooldownTimerHandles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged")
	TSubclassOf<AActor> RangedLightAttack = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged")
	TSubclassOf<AActor> RangedHeavyAttack = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged")
	TSubclassOf<AActor> RangedStunAttack = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged")
	TSubclassOf<AActor> MeleeLightDefense = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged")
	TSubclassOf<AActor> MeleeHeavyDefense = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged")
	TSubclassOf<AActor> MeleeStunDefense = nullptr;
};

