// Fill out your copyright notice in the Description page of Project Settings.

#include "SpecialAbilityComponent.h"
#include "../AvatarWoCtCCharacter.h"
#include "../AttackComponent.h"
#include "../AttackClasses/SpawnableAttack.h"

// Sets default values for this component's properties
USpecialAbilityComponent::USpecialAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bIsUsingCombo = true;
}


// Called when the game starts
void USpecialAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwningCharacter = Cast<AAvatarWoCtCCharacter>(GetOwner());
	ensure(OwningCharacter);
}


// Called every frame
void USpecialAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USpecialAbilityComponent::ActivateSpecial()
{
	bIsSpecialActive = true;

	if (OwningCharacter->GetRangedModeActive())	ThrowSpecialProjectile();
}

void USpecialAbilityComponent::DeactivateSpecial()
{
	bIsSpecialActive = false;
}

void USpecialAbilityComponent::ResetCombo()
{
	ComboCount = 0;
}

void USpecialAbilityComponent::IncrementCombo()
{
	ComboCount++;
}

void USpecialAbilityComponent::ThrowSpecialProjectile()
{
	TSubclassOf<AActor> spawn_proj_class = (GetIsComboMax()) ? MaxCombo_SpecialWeaponProjectile : SpecialWeaponProjectile;

	if (!spawn_proj_class) return;

	FVector spawn_loc = OwningCharacter->GetActorLocation() + OwningCharacter->GetActorForwardVector() * (float)EDistancePoints::DP_Close;
	FRotator spawn_rot = OwningCharacter->GetAimTargetRotator(spawn_loc);
	
	GetWorld()->SpawnActor<AActor>(spawn_proj_class, spawn_loc, spawn_rot);
}

void USpecialAbilityComponent::SetReferences(AAvatarWoCtCCharacter* owner, TSubclassOf<ASpawnableAttack> special, TSubclassOf<ASpawnableAttack> max_special)
{
	OwningCharacter = owner;
	SpecialWeaponProjectile = special;
	MaxCombo_SpecialWeaponProjectile = max_special;
}

