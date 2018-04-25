// Fill out your copyright notice in the Description page of Project Settings.

#include "Earthbending_SpecialAbilityComp.h"
#include "../AvatarWoCtCCharacter.h"
#include "../VitalsComponent.h"
#include "../AttackComponent.h"
#include "../AttackClasses/SpawnableAttack.h"
#include "GameFramework/ProjectileMovementComponent.h"

UEarthbending_SpecialAbilityComp::UEarthbending_SpecialAbilityComp()
{
	SetComponentTickEnabled(true);
	bIsSpecialToggle = false;
	bIsUsingCombo = false;
}

void UEarthbending_SpecialAbilityComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsSpecialActive)
	{
		OwningCharacter->GetVitalsComponent()->UpdateEnergy(-HoldBoulderEnergyRate * DeltaTime);

		if (OwningCharacter->GetVitalsComponent()->GetCurrentEnergy() == 0) DeactivateSpecial();

		if (HeldBoulder)
		{
			HeldBoulder->SetActorLocation(OwningCharacter->GetActorLocation() + OwningCharacter->GetActorForwardVector() * (float)EDistancePoints::DP_Close);
			HeldBoulder->SetActorRotation(OwningCharacter->GetActorRotation());
		}
	}
}

void UEarthbending_SpecialAbilityComp::ProcessAttackComponentData(EAttackType type)
{
	bool bIsDestroyed = false;

	if (!HeldBoulder) return;

	if (type == EAttackType::AT_Stun)
	{
		bIsDestroyed = HeldBoulder->ReduceHealth(4.f) <= 0.f;
	}
	else if (type == EAttackType::AT_Light)
	{
		bIsDestroyed = HeldBoulder->ReduceHealth(5.f) <= 0.f;
	}

	HeldBoulder->SetActorScale3D(FVector(HeldBoulder->GetDamage() / 20.f));

	if (bIsDestroyed || (type == EAttackType::AT_Heavy))
	{
		HeldBoulder->Destroy();
		HeldBoulder = nullptr;
		DeactivateSpecial();
	}
}

void UEarthbending_SpecialAbilityComp::ActivateSpecial()
{
	Super::ActivateSpecial();
	
	if (bIsSpecialActive && OwningCharacter->GetSpecialProjectile())
	{
		TSubclassOf<ASpawnableAttack> projectile_class = OwningCharacter->GetSpecialProjectile();
		FVector spawn_loc = OwningCharacter->GetActorLocation() + OwningCharacter->GetActorForwardVector() * (float)EDistancePoints::DP_Close;
		FRotator spawn_rot = OwningCharacter->GetActorRotation();
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		HeldBoulder = GetWorld()->SpawnActor<ASpawnableAttack>(projectile_class, spawn_loc, spawn_rot, param);

		UE_LOG(LogTemp, Warning, TEXT("spawning boulder...%s"), *GetNameSafe(HeldBoulder));

		if (HeldBoulder)
		{
			HeldBoulder->GetProjectileMoveComponent()->InitialSpeed = 0.f;
			HeldBoulder->GetProjectileMoveComponent()->MaxSpeed = 0.f;
		}

	}
}

void UEarthbending_SpecialAbilityComp::DeactivateSpecial()
{
	Super::DeactivateSpecial();
}

void UEarthbending_SpecialAbilityComp::AttackFinished(EAttackType Type)
{
	ProcessAttackComponentData(Type);
}

void UEarthbending_SpecialAbilityComp::DefenseFinished(EAttackType Type)
{
	ProcessAttackComponentData(Type);
}

bool UEarthbending_SpecialAbilityComp::CanUseAlternateAbility() const
{
	return bIsSpecialActive;
}
