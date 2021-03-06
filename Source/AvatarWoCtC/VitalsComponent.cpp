// Fill out your copyright notice in the Description page of Project Settings.

#include "VitalsComponent.h"
#include "AvatarWoCtCCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UVitalsComponent::UVitalsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UVitalsComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = BaseHealth;
	CurrentEnergy = BaseEnergy;

	OwningCharacter = Cast<AAvatarWoCtCCharacter>(GetOwner());
	ensure(OwningCharacter);
}


// Called every frame
void UVitalsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bWasAttackedRecently) UpdateHealth(HealthRegenPerSecond * DeltaTime);

	if (!bIsEnergyDisabled) CurrentEnergy = UpdateEnergy(EnergyRegenPerSecond * DeltaTime);

	if (!OwningCharacter->GetAttackTimerActive())
	{
		bool bIsJingCentered = UKismetMathLibrary::NearlyEqual_FloatFloat(Jing, 0.f, JingCenterTolerance);
		float NewJing = FMath::Abs(Jing) - (JingCenteringPerSecond * DeltaTime);

		Jing = (bIsJingCentered) ? 0.f : (Jing / FMath::Abs(Jing)) * NewJing;
	}

	if (Jing > JingPhaseThreshold) CurrentJingZone = (Jing > (MaxJing - 0.1f * MaxJing)) ? EJingPhase::JP_MaxPositive : EJingPhase::JP_Positive;
	else if (Jing < -JingPhaseThreshold) CurrentJingZone = (Jing < (-MaxJing + 0.1f * MaxJing)) ? EJingPhase::JP_MaxNegative : EJingPhase::JP_Negative;
	else CurrentJingZone = EJingPhase::JP_Neutral;
}

float UVitalsComponent::GetNormalizedCurrentJing() const
{
	return Jing + MaxJing;
}

float UVitalsComponent::GetNormalizedMaxJing() const
{
	return MaxJing + MaxJing;
}

float UVitalsComponent::UpdateHealth(float HealthDelta)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + HealthDelta, 0.f, BaseHealth);
	return CurrentHealth;
}

float UVitalsComponent::UpdateEnergy(float EnergyDelta)
{
	CurrentEnergy = FMath::Clamp(CurrentEnergy + EnergyDelta, 0.f, BaseEnergy);
	return CurrentEnergy;
}

float UVitalsComponent::UpdateJing(float JingDelta)
{
	Jing = FMath::Clamp(Jing + JingDelta, -MaxJing, MaxJing);
	return Jing;
}

