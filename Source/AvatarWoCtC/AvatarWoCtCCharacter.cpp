// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "AvatarWoCtCCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AAvatarWoCtCCharacter
AAvatarWoCtCCharacter::AAvatarWoCtCCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MovementState.bCanCrouch = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = fDefaultBoomLength; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AAvatarWoCtCCharacter::BeginPlay()
{
	Super::BeginPlay();

	fDefaultAirControl = GetCharacterMovement()->AirControl;
	fDefaultGravityScale = GetCharacterMovement()->GravityScale;

	bCanDoubleJump = BendingDiscipline == EBendingDisciplineType::BDT_Air;
	bCanHover = (BendingDiscipline == EBendingDisciplineType::BDT_Air) || (BendingDiscipline == EBendingDisciplineType::BDT_Fire);

	JumpMaxCount = JumpMaxCount + bCanDoubleJump;

	fDefaultBoomLength = CameraBoom->TargetArmLength;

	fDefaultMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void AAvatarWoCtCCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bCanHover) CheckHover();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAvatarWoCtCCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAvatarWoCtCCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AAvatarWoCtCCharacter::EndJump);

	PlayerInputComponent->BindAxis("MoveForward", this, &AAvatarWoCtCCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAvatarWoCtCCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &AAvatarWoCtCCharacter::RotateYaw);
	PlayerInputComponent->BindAxis("TurnRate", this, &AAvatarWoCtCCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AAvatarWoCtCCharacter::RotatePitch);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AAvatarWoCtCCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("RangedMode", IE_Pressed, this, &AAvatarWoCtCCharacter::StartRangedMode);
	PlayerInputComponent->BindAction("RangedMode", IE_Released, this, &AAvatarWoCtCCharacter::EndRangedMode);

	PlayerInputComponent->BindAction("CenterCamera", IE_Pressed, this, &AAvatarWoCtCCharacter::CenterCamera);

	PlayerInputComponent->BindAction("GuardMode", IE_Pressed, this, &AAvatarWoCtCCharacter::StartGuardMode);
	PlayerInputComponent->BindAction("GuardMode", IE_Released, this, &AAvatarWoCtCCharacter::EndGuardMode);
}

void AAvatarWoCtCCharacter::CheckHover()
{
	if (bJumpHeld && GetVelocity().Z < 0.f && GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
	{
		GetCharacterMovement()->GravityScale = fHoverGravityScale;
		GetCharacterMovement()->AirControl = fHoverAirControl;
	}
}

void AAvatarWoCtCCharacter::CenterCamera()
{
	if (bRangedModeActive)
	{
		float currentOffsetFactor = FollowCamera->GetRelativeTransform().GetLocation().Y;
		currentOffsetFactor = -1.f * (currentOffsetFactor / FMath::Abs(currentOffsetFactor));
		FollowCamera->SetRelativeLocation(FVector(0.f, fCameraShiftOffset * currentOffsetFactor, 0.f));
	}
	else
	{
		APlayerController* PC = Cast<APlayerController>(Controller);
		if (PC) PC->SetControlRotation(GetActorRotation());
	}
}

void AAvatarWoCtCCharacter::StartJump()
{
	bJumpHeld = true;
	Jump();
}

void AAvatarWoCtCCharacter::EndJump()
{
	bJumpHeld = false;
	StopJumping();

	GetCharacterMovement()->GravityScale = fDefaultGravityScale;
	GetCharacterMovement()->AirControl = fDefaultAirControl;
}

void AAvatarWoCtCCharacter::StartRangedMode()
{
	bRangedModeActive = !bRangedModeActive;

	if (bRangedModeActive) CenterCamera(); 

	CameraBoom->TargetArmLength = (bRangedModeActive) ? fRangedBoomLength : fDefaultBoomLength;
	FollowCamera->SetRelativeLocation(FVector(0.f, (bRangedModeActive) ? fCameraShiftOffset : 0.f, 0.f));

	bUseControllerRotationYaw = bRangedModeActive;
}

void AAvatarWoCtCCharacter::EndRangedMode()
{
	if (!bIsRangedToggle && bRangedModeActive) StartRangedMode();
}

void AAvatarWoCtCCharacter::StartGuardMode()
{
	bGuardModeActive = !bGuardModeActive;

	GetCharacterMovement()->MaxWalkSpeed = fDefaultMoveSpeed * ((bGuardModeActive) ? fGuardModeSpeedFactor : 1.f);
}

void AAvatarWoCtCCharacter::EndGuardMode()
{
	if (!bIsGuardToggle && bGuardModeActive) StartGuardMode();
}

void AAvatarWoCtCCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAvatarWoCtCCharacter::RotateYaw(float Rate)
{
	AddControllerYawInput(Rate);
}

void AAvatarWoCtCCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAvatarWoCtCCharacter::RotatePitch(float Rate)
{
	AddControllerPitchInput(Rate);
}

void AAvatarWoCtCCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AAvatarWoCtCCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
