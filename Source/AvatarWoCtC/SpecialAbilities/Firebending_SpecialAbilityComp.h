// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpecialAbilities/SpecialAbilityComponent.h"
#include "Firebending_SpecialAbilityComp.generated.h"

/**
 * 
 */
UCLASS()
class AVATARWOCTC_API UFirebending_SpecialAbilityComp : public USpecialAbilityComponent
{
	GENERATED_BODY()

	void EndBurnChi();

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Firebending")
	bool bIsBurnChiActive = false;

	UPROPERTY(BlueprintReadOnly, Category = "Firebending")
	float BurnChiJingRate = 10.f;
	
public:
	UFirebending_SpecialAbilityComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActivateSpecial() override;
	virtual void DeactivateSpecial() override;	

	virtual void DefenseFinished(EAttackType Type) override;
};
