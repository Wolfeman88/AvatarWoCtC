// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpecialAbilities/SpecialAbilityComponent.h"
#include "Waterbending_SpecialAbilityComp.generated.h"

/**
 * 
 */
UCLASS()
class AVATARWOCTC_API UWaterbending_SpecialAbilityComp : public USpecialAbilityComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Waterbending")
	float IceModeEnergyRate = 15.f;

public:
	UWaterbending_SpecialAbilityComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActivateSpecial() override;
	virtual void DeactivateSpecial() override;

	virtual bool CanUseAlternateAbility() const override;
};
