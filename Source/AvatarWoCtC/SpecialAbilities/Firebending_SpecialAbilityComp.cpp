// Fill out your copyright notice in the Description page of Project Settings.

#include "Firebending_SpecialAbilityComp.h"
#include "../AvatarWoCtCCharacter.h"
#include "../VitalsComponent.h"

UFirebending_SpecialAbilityComp::UFirebending_SpecialAbilityComp()
{
	bIsSpecialToggle = true;
	bIsUsingCombo = true;
}

void UFirebending_SpecialAbilityComp::ActivateSpecial()
{
	Super::ActivateSpecial();

	ComboCount = MaxCombo;
}

void UFirebending_SpecialAbilityComp::DeactivateSpecial()
{
	Super::DeactivateSpecial();

	ComboCount = 0;
}


