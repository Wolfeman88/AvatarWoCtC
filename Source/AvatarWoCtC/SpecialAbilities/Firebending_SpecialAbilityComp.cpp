// Fill out your copyright notice in the Description page of Project Settings.

#include "Firebending_SpecialAbilityComp.h"


void UFirebending_SpecialAbilityComp::ActivateSpecial()
{
	Super::ActivateSpecial();

	UE_LOG(LogTemp, Warning, TEXT("activating special in child class..."));
}

void UFirebending_SpecialAbilityComp::DeactivateSpecial()
{
	Super::DeactivateSpecial();

	UE_LOG(LogTemp, Warning, TEXT("deactivating special in child class..."));
}


