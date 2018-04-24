// Fill out your copyright notice in the Description page of Project Settings.

#include "Airbending_SpecialAbilityComp.h"
#include "../AvatarWoCtCCharacter.h"
#include "../VitalsComponent.h"

UAirbending_SpecialAbilityComp::UAirbending_SpecialAbilityComp()
{
	SetComponentTickEnabled(true);
	bIsUsingCombo = false;
	bIsSpecialToggle = true;
}

void UAirbending_SpecialAbilityComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsEvasionActive)
	{
		OwningCharacter->ShiftJing(EvasionJingRate * DeltaTime);
		OwningCharacter->GetVitalsComponent()->UpdateEnergy(EvasionEnergyRate * DeltaTime);

		if (OwningCharacter->GetVitalsComponent()->GetCurrentJingZone() == EJingPhase::JP_Neutral) EndEvasion();
	}
}

void UAirbending_SpecialAbilityComp::EndEvasion()
{
	bIsEvasionActive = false;
	OwningCharacter->SetRollCooldown(BaseRollCooldown);
}

void UAirbending_SpecialAbilityComp::ActivateSpecial()
{
	Super::ActivateSpecial();

	if (OwningCharacter->GetVitalsComponent()->GetCurrentJingZone() == EJingPhase::JP_MaxNegative)
	{
		bIsEvasionActive = true;
		OwningCharacter->GetVitalsComponent()->UpdateEnergy(EvasionEnergyRate);
		BaseRollCooldown = OwningCharacter->GetRollCooldown();
		OwningCharacter->SetRollCooldown(0.f);
	}
}

void UAirbending_SpecialAbilityComp::DeactivateSpecial()
{
	Super::DeactivateSpecial();
}

void UAirbending_SpecialAbilityComp::AttackFinished(EAttackType Type)
{
	Super::AttackFinished(Type);

	EndEvasion();
}
