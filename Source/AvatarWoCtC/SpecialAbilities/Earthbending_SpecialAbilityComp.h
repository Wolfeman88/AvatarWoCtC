// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpecialAbilities/SpecialAbilityComponent.h"
#include "Earthbending_SpecialAbilityComp.generated.h"

/**
 * 
 */
UCLASS()
class AVATARWOCTC_API UEarthbending_SpecialAbilityComp : public USpecialAbilityComponent
{
	GENERATED_BODY()

	void ProcessAttackComponentData(EAttackType type);

	ASpawnableAttack* HeldBoulder = nullptr;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Earthbending")
	float HoldBoulderEnergyRate = 20.f;

public:
	UEarthbending_SpecialAbilityComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActivateSpecial() override;
	virtual void DeactivateSpecial() override;

	virtual void AttackFinished(EAttackType Type) override;
	virtual void DefenseFinished(EAttackType Type) override;

	UFUNCTION(BlueprintCallable, Category = "Special")
	virtual bool CanUseAlternateAbility() const override;
};
