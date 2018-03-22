// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeAttackComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "AvatarWoCtCCharacter.h"

// Sets default values for this component's properties
UMeleeAttackComponent::UMeleeAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called every frame
void UMeleeAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMeleeAttackComponent::BeginPlay()
{
	OwningCharacter = Cast<AAvatarWoCtCCharacter>(GetOwner());
	ensure(OwningCharacter);
}

void UMeleeAttackComponent::ActivateMeleeAbility(EAttackType NewAttack, FMeleeAttack AttackData)
{
	if (OwningCharacter->GetWorldTimerManager().IsTimerActive(AttackTimerHandle))
	{
		QueuedAttack = NewAttack;
		QueuedAttackData = AttackData;
		return;
	}

	float speedMultiplier = 1.f;

	switch (NewAttack)
	{
	case EAttackType::AT_Light:
		OwningCharacter->GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &UMeleeAttackComponent::Light, LightAttackActivationTime);
		speedMultiplier = LightAttackSpeedFactor;
		break;
	case EAttackType::AT_Heavy:
		OwningCharacter->GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &UMeleeAttackComponent::Heavy, HeavyAttackActivationTime);
		speedMultiplier = HeavyAttackSpeedFactor;
		break;
	case EAttackType::AT_Stun:
		OwningCharacter->GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &UMeleeAttackComponent::Stun, StunAttackActivationTime);
		speedMultiplier = StunAttackSpeedFactor;
		break;
	case EAttackType::AT_None:
	default:
		break;
	}

	CurrentAttack = NewAttack;
	CurrentAttackData = AttackData;

	QueuedAttack = EAttackType::AT_None;
	QueuedAttackData = FMeleeAttack();

	OwningCharacter->ChangeSpeedWhileActivatingAbility(speedMultiplier);
}

FVector UMeleeAttackComponent::GetRelativeVectorOffset(FVector Offset)
{
	FVector ActorLocation = GetOwner()->GetActorLocation();

	FVector OffsetX, OffsetY, OffsetZ;
	OffsetX = GetOwner()->GetActorForwardVector() * Offset.X;
	OffsetY = GetOwner()->GetActorRightVector() * Offset.Y;
	OffsetZ = GetOwner()->GetActorUpVector() * Offset.Z;

	return ActorLocation + OffsetX + OffsetY + OffsetZ;
}

void UMeleeAttackComponent::EndAttack()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	if (QueuedAttack != EAttackType::AT_None) ActivateMeleeAbility(QueuedAttack, QueuedAttackData);
	else
	{
		CurrentAttack = EAttackType::AT_None;
		CurrentAttackData = FMeleeAttack();
		OwningCharacter->ChangeSpeedWhileActivatingAbility(1.f);
	}
}

void UMeleeAttackComponent::Light()
{
	TArray<FMeleeTrace> traces = CurrentAttackData.Traces;

	for (int i = 0; i < traces.Num(); i++)
	{
		FVector Source = GetRelativeVectorOffset(traces[i].TraceSource);
		FVector End = GetRelativeVectorOffset(traces[i].TraceEnd);

		DrawDebugLine(GetWorld(), Source, End, FColor::Blue, false, 5.f, 0, 5.f);
	}

	EndAttack();
}

void UMeleeAttackComponent::Heavy()
{
	TArray<FMeleeTrace> traces = CurrentAttackData.Traces;

	for (int i = 0; i < traces.Num(); i++)
	{
		FVector Source = GetRelativeVectorOffset(traces[i].TraceSource);
		FVector End = GetRelativeVectorOffset(traces[i].TraceEnd);

		DrawDebugLine(GetWorld(), Source, End, FColor::Red, false, 5.f, 0, 20.f);
	}

	EndAttack();
}

void UMeleeAttackComponent::Stun()
{
	TArray<FMeleeTrace> traces = CurrentAttackData.Traces;

	for (int i = 0; i < traces.Num(); i++)
	{
		FVector Source = GetRelativeVectorOffset(traces[i].TraceSource);
		FVector End = GetRelativeVectorOffset(traces[i].TraceEnd);

		DrawDebugLine(GetWorld(), Source, End, FColor::Green, false, 5.f, 0, 2.5f);
	}

	EndAttack();
}

