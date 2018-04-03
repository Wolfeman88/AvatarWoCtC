// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "AvatarWoCtCCharacter.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttackComponent::BeginPlay()
{
	OwningCharacter = Cast<AAvatarWoCtCCharacter>(GetOwner());
	ensure(OwningCharacter);
}

void UAttackComponent::ActivateMeleeAbility(EAttackType NewAttack, FMeleeAttack AttackData)
{
	if (OwningCharacter->GetWorldTimerManager().IsTimerActive(AttackTimerHandle))
	{
		M_QueuedAttack = NewAttack;
		M_QueuedAttackData = AttackData;
		return;
	}

	float speedMultiplier = 1.f;

	switch (NewAttack)
	{
	case EAttackType::AT_Light:
		OwningCharacter->GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &UAttackComponent::M_Light, LightAttackActivationTime);
		speedMultiplier = LightAttackSpeedFactor;
		break;
	case EAttackType::AT_Heavy:
		OwningCharacter->GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &UAttackComponent::M_Heavy, HeavyAttackActivationTime);
		speedMultiplier = HeavyAttackSpeedFactor;
		break;
	case EAttackType::AT_Stun:
		OwningCharacter->GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &UAttackComponent::M_Stun, StunAttackActivationTime);
		speedMultiplier = StunAttackSpeedFactor;
		break;
	case EAttackType::AT_None:
	default:
		break;
	}

	M_CurrentAttack = NewAttack;
	M_CurrentAttackData = AttackData;

	M_QueuedAttack = EAttackType::AT_None;
	M_QueuedAttackData = FMeleeAttack();

	OwningCharacter->ChangeSpeedWhileActivatingAbility(speedMultiplier);
}

void UAttackComponent::ActivateRangedAbility(TSubclassOf<AActor> AttackToSpawn, FVector OriginationOffset)
{
	if (OwningCharacter->GetWorldTimerManager().IsTimerActive(AttackTimerHandle))
	{
		R_QueuedAttack = AttackToSpawn;
		R_QueuedOffset = OriginationOffset;
		return;
	}

	float speedMultiplier = 1.f;

	if (AttackToSpawn)
	{
		// ranged attack enum and actor class
		OwningCharacter->GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &UAttackComponent::R_Attack, 0.75f);
		speedMultiplier = 0.1f;
	}

	R_CurrentAttack = AttackToSpawn;
	R_CurrentOffset = OriginationOffset;

	R_QueuedAttack = nullptr;
	R_QueuedOffset = FVector::ZeroVector;

	OwningCharacter->ChangeSpeedWhileActivatingAbility(speedMultiplier);
}

void UAttackComponent::R_Attack()
{
	FVector SpawnLoc = GetRelativeVectorOffset(R_CurrentOffset);
	FRotator SpawnRot = OwningCharacter->GetControlRotation();
	FActorSpawnParameters params;
	params.Owner = OwningCharacter;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* SpawnRef = GetWorld()->SpawnActor<AActor>(R_CurrentAttack, SpawnLoc, SpawnRot, params);

	GetOwner()->GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	if (R_QueuedAttack != nullptr) ActivateRangedAbility(R_QueuedAttack, R_QueuedOffset);
	else
	{
		R_CurrentAttack = nullptr;
		R_QueuedOffset = FVector::ZeroVector;
		OwningCharacter->ChangeSpeedWhileActivatingAbility(1.f);
	}

	SpawnRef = nullptr;
}

FVector UAttackComponent::GetRelativeVectorOffset(FVector Offset)
{
	FVector ActorLocation = GetOwner()->GetActorLocation();

	FVector OffsetX, OffsetY, OffsetZ;
	OffsetX = GetOwner()->GetActorForwardVector() * Offset.X;
	OffsetY = GetOwner()->GetActorRightVector() * Offset.Y;
	OffsetZ = GetOwner()->GetActorUpVector() * Offset.Z;

	return ActorLocation + OffsetX + OffsetY + OffsetZ;
}

void UAttackComponent::EndAttack()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	if (M_QueuedAttack != EAttackType::AT_None) ActivateMeleeAbility(M_QueuedAttack, M_QueuedAttackData);
	else
	{
		M_CurrentAttack = EAttackType::AT_None;
		M_CurrentAttackData = FMeleeAttack();
		OwningCharacter->ChangeSpeedWhileActivatingAbility(1.f);
	}
}

void UAttackComponent::M_Light()
{
	TArray<FMeleeTrace> traces = M_CurrentAttackData.Traces;

	for (int i = 0; i < traces.Num(); i++)
	{
		FVector Source = GetRelativeVectorOffset(traces[i].TraceSource);
		FVector End = GetRelativeVectorOffset(traces[i].TraceEnd);

		DrawDebugLine(GetWorld(), Source, End, FColor::Blue, false, 5.f, 0, 5.f);
	}

	EndAttack();
}

void UAttackComponent::M_Heavy()
{
	TArray<FMeleeTrace> traces = M_CurrentAttackData.Traces;

	for (int i = 0; i < traces.Num(); i++)
	{
		FVector Source = GetRelativeVectorOffset(traces[i].TraceSource);
		FVector End = GetRelativeVectorOffset(traces[i].TraceEnd);

		DrawDebugLine(GetWorld(), Source, End, FColor::Red, false, 5.f, 0, 20.f);
	}

	EndAttack();
}

void UAttackComponent::M_Stun()
{
	TArray<FMeleeTrace> traces = M_CurrentAttackData.Traces;

	for (int i = 0; i < traces.Num(); i++)
	{
		FVector Source = GetRelativeVectorOffset(traces[i].TraceSource);
		FVector End = GetRelativeVectorOffset(traces[i].TraceEnd);

		DrawDebugLine(GetWorld(), Source, End, FColor::Green, false, 5.f, 0, 2.5f);
	}

	EndAttack();
}

