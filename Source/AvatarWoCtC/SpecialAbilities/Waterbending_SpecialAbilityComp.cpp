// Fill out your copyright notice in the Description page of Project Settings.

#include "Waterbending_SpecialAbilityComp.h"
#include "../AvatarWoCtCCharacter.h"
#include "../VitalsComponent.h"

UWaterbending_SpecialAbilityComp::UWaterbending_SpecialAbilityComp()
{
	SetComponentTickEnabled(true);
	bIsUsingCombo = false;
	bIsSpecialToggle = false;
}

void UWaterbending_SpecialAbilityComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsSpecialActive)
	{
		OwningCharacter->GetVitalsComponent()->UpdateEnergy(-IceModeEnergyRate * DeltaTime);

		if (OwningCharacter->GetVitalsComponent()->GetCurrentEnergy() == 0.f)
		{
			DeactivateSpecial();
		}
	}
}

void UWaterbending_SpecialAbilityComp::ActivateSpecial()
{
	Super::ActivateSpecial();

}

void UWaterbending_SpecialAbilityComp::DeactivateSpecial()
{
	Super::DeactivateSpecial();

}

bool UWaterbending_SpecialAbilityComp::CanUseAlternateAbility() const
{
	return bIsSpecialActive;
}
