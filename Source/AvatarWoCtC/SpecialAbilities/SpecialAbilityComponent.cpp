// Fill out your copyright notice in the Description page of Project Settings.

#include "SpecialAbilityComponent.h"


// Sets default values for this component's properties
USpecialAbilityComponent::USpecialAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpecialAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USpecialAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USpecialAbilityComponent::ActivateSpecial()
{
	SpecialActive = true;
}

void USpecialAbilityComponent::DeactivateSpecial()
{
	SpecialActive = false;
}

