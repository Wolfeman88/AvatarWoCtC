// Fill out your copyright notice in the Description page of Project Settings.

#include "Firebending_SpecialAbilityComp.h"
#include "../AvatarWoCtCCharacter.h"
#include "../VitalsComponent.h"

UFirebending_SpecialAbilityComp::UFirebending_SpecialAbilityComp()
{
	SetComponentTickEnabled(true);
	bIsSpecialToggle = true;
	bIsUsingCombo = true;
}

void UFirebending_SpecialAbilityComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsBurnChiActive)
	{
		OwningCharacter->ShiftJing(-BurnChiJingRate * DeltaTime);

		if (OwningCharacter->GetVitalsComponent()->GetCurrentJingZone() == EJingPhase::JP_Neutral) EndBurnChi();
	}
}

void UFirebending_SpecialAbilityComp::EndBurnChi()
{
	bIsBurnChiActive = false;
	ComboCount = 0;
}


void UFirebending_SpecialAbilityComp::ActivateSpecial()
{
	Super::ActivateSpecial();

	if (OwningCharacter->GetVitalsComponent()->GetCurrentJingZone() == EJingPhase::JP_MaxPositive)
	{
		bIsBurnChiActive = true;
		ComboCount = MaxCombo;
	}
}

void UFirebending_SpecialAbilityComp::DeactivateSpecial()
{
	Super::DeactivateSpecial();

}

void UFirebending_SpecialAbilityComp::DefenseFinished(EAttackType Type)
{
	Super::DefenseFinished(Type);

	EndBurnChi();
}


