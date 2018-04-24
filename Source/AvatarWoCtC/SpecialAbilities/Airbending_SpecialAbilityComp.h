// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpecialAbilities/SpecialAbilityComponent.h"
#include "Airbending_SpecialAbilityComp.generated.h"

/**
 * 
 */
UCLASS()
class AVATARWOCTC_API UAirbending_SpecialAbilityComp : public USpecialAbilityComponent
{
	GENERATED_BODY()

	void EndEvasion();

	float BaseRollCooldown = 0.f;
	
protected:

	UPROPERTY(BlueprintReadOnly, Category = "Airbending")
	bool bIsEvasionActive = false;

	UPROPERTY(BlueprintReadOnly, Category = "Airbending")
	float EvasionJingRate = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Airbending")
	float EvasionEnergyRate = 100.f;

public:
	UAirbending_SpecialAbilityComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActivateSpecial() override;
	virtual void DeactivateSpecial() override;

	virtual void AttackFinished(EAttackType Type) override;
};
