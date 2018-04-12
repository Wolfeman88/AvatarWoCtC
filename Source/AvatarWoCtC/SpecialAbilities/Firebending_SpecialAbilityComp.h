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
	
public:
	virtual void ActivateSpecial() override;
	virtual void DeactivateSpecial() override;
	
	
};
