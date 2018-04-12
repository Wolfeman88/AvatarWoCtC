// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "AvatarWoCtCCharacter.h"
#include "Camera/CameraComponent.h"
#include "Engine/DataTable.h"
#include "./AttackClasses/SpawnableAttack.h"

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
		ClearQueue();
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

void UAttackComponent::ActivateRangedAbility(TSubclassOf<ASpawnableAttack> AttackToSpawn, FVector OriginationOffset)
{
	if (OwningCharacter->GetWorldTimerManager().IsTimerActive(AttackTimerHandle))
	{
		ClearQueue();
		R_QueuedAttack = AttackToSpawn;
		R_QueuedOffset = OriginationOffset;
		return;
	}

	float speedMultiplier = 1.f;

	if (AttackToSpawn)
	{
		EAttackType temp_type = Cast<ASpawnableAttack>(AttackToSpawn->GetDefaultObject())->Type;
		float activation_time = 1.f;

		switch (temp_type)
		{
		case EAttackType::AT_Light:
			activation_time = LightAttackActivationTime;
			speedMultiplier = LightAttackSpeedFactor;
			break;
		case EAttackType::AT_Heavy:
			activation_time = HeavyAttackActivationTime;
			speedMultiplier = HeavyAttackSpeedFactor;
			break;
		case EAttackType::AT_Stun:
			activation_time = StunAttackActivationTime;
			speedMultiplier = StunAttackSpeedFactor;
			break;
		case EAttackType::AT_None:
		default:
			break;
		}

		OwningCharacter->GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &UAttackComponent::R_Attack, activation_time);
	}

	R_CurrentAttack = AttackToSpawn;
	R_CurrentOffset = OriginationOffset;

	R_QueuedAttack = nullptr;
	R_QueuedOffset = FVector::ZeroVector;

	OwningCharacter->ChangeSpeedWhileActivatingAbility(speedMultiplier);
}

void UAttackComponent::ActivateMeleeDefenseAbility(TSubclassOf<ASpawnableAttack> AttackToSpawn, float OriginationOffset)
{
	if (OwningCharacter->GetWorldTimerManager().IsTimerActive(AttackTimerHandle))
	{
		ClearQueue();
		DM_QueuedAttack = AttackToSpawn;
		DM_QueuedOffset = OriginationOffset;
		return;
	}

	float speedMultiplier = 1.f;

	if (AttackToSpawn)
	{
		EAttackType temp_type = Cast<ASpawnableAttack>(AttackToSpawn->GetDefaultObject())->Type;
		float activation_time = 1.f;

		switch (temp_type)
		{
		case EAttackType::AT_Light:
			activation_time = LightAttackActivationTime;
			speedMultiplier = LightAttackSpeedFactor;
			break;
		case EAttackType::AT_Heavy:
			activation_time = HeavyAttackActivationTime;
			speedMultiplier = HeavyAttackSpeedFactor;
			break;
		case EAttackType::AT_Stun:
			activation_time = StunAttackActivationTime;
			speedMultiplier = StunAttackSpeedFactor;
			break;
		case EAttackType::AT_None:
		default:
			break;
		}

		OwningCharacter->GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &UAttackComponent::M_Defense, 1.f);
	}

	DM_CurrentAttack = AttackToSpawn;
	DM_CurrentOffset = OriginationOffset;

	DM_QueuedAttack = nullptr;
	DM_QueuedOffset = 0.f;

	OwningCharacter->ChangeSpeedWhileActivatingAbility(speedMultiplier);
}

void UAttackComponent::ActivateRangedDefenseAbility(TSubclassOf<ASpawnableAttack> AttackToSpawn, float OriginationOffset)
{
	if (OwningCharacter->GetWorldTimerManager().IsTimerActive(AttackTimerHandle))
	{
		ClearQueue();
		DR_QueuedAttack = AttackToSpawn;
		DR_QueuedOffset = OriginationOffset;
		return;
	}

	float speedMultiplier = 1.f;

	if (AttackToSpawn)
	{
		EAttackType temp_type = Cast<ASpawnableAttack>(AttackToSpawn->GetDefaultObject())->Type;
		float activation_time = 1.f;

		switch (temp_type)
		{
		case EAttackType::AT_Light:
			activation_time = LightAttackActivationTime;
			speedMultiplier = LightAttackSpeedFactor;
			break;
		case EAttackType::AT_Heavy:
			activation_time = HeavyAttackActivationTime;
			speedMultiplier = HeavyAttackSpeedFactor;
			break;
		case EAttackType::AT_Stun:
			activation_time = StunAttackActivationTime;
			speedMultiplier = StunAttackSpeedFactor;
			break;
		case EAttackType::AT_None:
		default:
			break;
		}

		OwningCharacter->GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &UAttackComponent::R_Defense, 1.f);
	}

	DR_CurrentAttack = AttackToSpawn;
	DR_CurrentOffset = OriginationOffset;

	DR_QueuedAttack = nullptr;
	DR_QueuedOffset = 0.f;

	OwningCharacter->ChangeSpeedWhileActivatingAbility(speedMultiplier);
}

FMeleeAttack UAttackComponent::GetRandomAttackData(EAttackType AttackStyle)
{
	FMeleeAttack attack = FMeleeAttack();
	UDataTable* RequestDataTableRef = nullptr;

	switch (AttackStyle)
	{
	case EAttackType::AT_Light:
		RequestDataTableRef = LightAttackData;
		break;
	case EAttackType::AT_Heavy:
		RequestDataTableRef = HeavyAttackData;
		break;
	case EAttackType::AT_Stun:
		RequestDataTableRef = StunAttackData;
		break;
	case EAttackType::AT_None:
	default:
		break;
	}

	if (RequestDataTableRef != nullptr)
	{
		TArray<FName> RowNames = RequestDataTableRef->GetRowNames();

		if (RowNames.Num() != 0)
		{
			int32 index = FMath::RandRange(0, RowNames.Num() - 1);
			attack = *RequestDataTableRef->FindRow<FMeleeAttack>(RowNames[index], RowNames[index].ToString());
		}
	}

	return attack;
}

void UAttackComponent::R_Attack()
{
	FVector SpawnLoc = GetRelativeVectorOffset(R_CurrentOffset);
	FRotator SpawnRot = OwningCharacter->GetAimTargetRotator(SpawnLoc);
	FActorSpawnParameters params;
	params.Owner = OwningCharacter;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(R_CurrentAttack, SpawnLoc, SpawnRot, params);

	EndAttack();
}

void UAttackComponent::M_Defense()
{
	FVector SpawnLoc = GetRelativeVectorOffset(FVector(DM_CurrentOffset, 0.f, 0.f));
	FRotator SpawnRot = OwningCharacter->GetActorForwardVector().ToOrientationRotator();
	FActorSpawnParameters params;
	params.Owner = OwningCharacter;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(DM_CurrentAttack, SpawnLoc, SpawnRot, params);

	EndAttack();
}

void UAttackComponent::R_Defense()
{
	FVector SpawnLoc = OwningCharacter->GetAimTargetEndLocation();
	FRotator SpawnRot = OwningCharacter->GetControlRotation();
	FActorSpawnParameters params;
	params.Owner = OwningCharacter;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(DR_CurrentAttack, SpawnLoc, SpawnRot, params);

	EndAttack();
}

FVector UAttackComponent::ConvertMeleeVector(FMeleeVector EnumOffset)
{
	FVector rVec = FVector::ZeroVector;

	rVec.X = (float)EnumOffset.ForwardOffset;
	rVec.Y = (float)EnumOffset.RightOffset - (float)EWidthPoints::DP_None;
	rVec.Z = (float)EnumOffset.UpOffset - (float)EWidthPoints::DP_None;

	return rVec;
}

FVector UAttackComponent::GetRelativeVectorOffset(FVector Offset)
{
	FVector ActorLocation = OwningCharacter->GetActorLocation();

	FVector OffsetX, OffsetY, OffsetZ;
	OffsetX = OwningCharacter->GetActorForwardVector() * Offset.X;
	OffsetY = OwningCharacter->GetActorRightVector() * Offset.Y;
	OffsetZ = OwningCharacter->GetActorUpVector() * Offset.Z;

	return ActorLocation + OffsetX + OffsetY + OffsetZ;
}

void UAttackComponent::EndAttack()
{
	EAttackType temp_type = M_CurrentAttack;

	if (R_CurrentAttack) temp_type = Cast<ASpawnableAttack>(R_CurrentAttack.GetDefaultObject())->Type;
	else if (DM_CurrentAttack) temp_type = Cast<ASpawnableAttack>(DM_CurrentAttack.GetDefaultObject())->Type;
	else if (DR_CurrentAttack) temp_type = Cast<ASpawnableAttack>(DR_CurrentAttack.GetDefaultObject())->Type;

	float Jing = 0.f;

	switch (temp_type)
	{
	case EAttackType::AT_Light:
		Jing = LightAbilityJingDelta;
		break;
	case EAttackType::AT_Heavy:
		Jing = HeavyAbilityJingDelta;
		break;
	case EAttackType::AT_Stun:
		Jing = StunAbilityJingDelta;
		break;
	case EAttackType::AT_None:
	default:
		break;
	}

	if (DM_CurrentAttack || DR_CurrentAttack) Jing = -Jing;

	OwningCharacter->ShiftJing(Jing);
	
	OwningCharacter->GetWorldTimerManager().ClearTimer(AttackTimerHandle);

	if (M_QueuedAttack != EAttackType::AT_None) ActivateMeleeAbility(M_QueuedAttack, M_QueuedAttackData);
	else if (R_QueuedAttack != nullptr) ActivateRangedAbility(R_QueuedAttack, R_QueuedOffset);
	else if (DM_QueuedAttack != nullptr) ActivateMeleeDefenseAbility(DM_QueuedAttack, DM_QueuedOffset);
	else if (DR_QueuedAttack != nullptr) ActivateRangedDefenseAbility(DR_QueuedAttack, DR_QueuedOffset);
	else
	{
		M_CurrentAttack = EAttackType::AT_None;
		M_CurrentAttackData = FMeleeAttack();
		R_CurrentAttack = nullptr;
		R_CurrentOffset = FVector::ZeroVector;
		DM_CurrentAttack = nullptr;
		DM_CurrentOffset = 0.f;
		DR_CurrentAttack = nullptr;
		DR_CurrentOffset = 0.f;
		OwningCharacter->ChangeSpeedWhileActivatingAbility(1.f);
	}
}

void UAttackComponent::ClearQueue()
{
	M_QueuedAttack = EAttackType::AT_None;
	M_QueuedAttackData = FMeleeAttack();
	R_QueuedAttack = nullptr;
	R_QueuedOffset = FVector::ZeroVector;
	DM_QueuedAttack = nullptr;
	DM_QueuedOffset = 0.f;
	DR_QueuedAttack = nullptr;
	DR_QueuedOffset = 0.f;
}

bool UAttackComponent::GetAttackTimerActive() const
{
	return OwningCharacter->GetWorldTimerManager().IsTimerActive(AttackTimerHandle);
}

void UAttackComponent::M_Light()
{
	TArray<FMeleeTrace> traces = M_CurrentAttackData.Traces;

	for (int i = 0; i < traces.Num(); i++)
	{
		FVector Source = GetRelativeVectorOffset(ConvertMeleeVector(traces[i].Source));
		FVector End = GetRelativeVectorOffset(ConvertMeleeVector(traces[i].End));

		DrawDebugLine(GetWorld(), Source, End, FColor::Blue, false, 5.f, 0, 5.f);
	}

	EndAttack();
}

void UAttackComponent::M_Heavy()
{
	TArray<FMeleeTrace> traces = M_CurrentAttackData.Traces;

	for (int i = 0; i < traces.Num(); i++)
	{
		FVector Source = GetRelativeVectorOffset(ConvertMeleeVector(traces[i].Source));
		FVector End = GetRelativeVectorOffset(ConvertMeleeVector(traces[i].End));

		DrawDebugLine(GetWorld(), Source, End, FColor::Red, false, 5.f, 0, 20.f);
	}

	EndAttack();
}

void UAttackComponent::M_Stun()
{
	TArray<FMeleeTrace> traces = M_CurrentAttackData.Traces;

	for (int i = 0; i < traces.Num(); i++)
	{
		FVector Source = GetRelativeVectorOffset(ConvertMeleeVector(traces[i].Source));
		FVector End = GetRelativeVectorOffset(ConvertMeleeVector(traces[i].End));

		DrawDebugLine(GetWorld(), Source, End, FColor::Green, false, 5.f, 0, 2.5f);
	}

	EndAttack();
}
