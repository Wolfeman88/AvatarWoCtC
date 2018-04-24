// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VitalsComponent.generated.h"

UENUM(BlueprintType)
enum class EJingPhase : uint8
{
	JP_MaxNegative		UMETA(DisplayName = "Maximum Negative"),
	JP_Negative			UMETA(DisplayName = "Negative Jing"),
	JP_Neutral			UMETA(DisplayName = "Neutral Jing"),
	JP_Positive			UMETA(DisplayName = "Positive Jing"),
	JP_MaxPositive		UMETA(DisplayName = "Maximum Positive")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AVATARWOCTC_API UVitalsComponent : public UActorComponent
{
	GENERATED_BODY()

	class AAvatarWoCtCCharacter* OwningCharacter = nullptr;

public:	
	// Sets default values for this component's properties
	UVitalsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float BaseHealth = 100.f;
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Energy")
	float BaseEnergy = 100.f;
	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	float CurrentEnergy;

	UPROPERTY(EditDefaultsOnly, Category = "Jing")
	float MaxJing = 50.f;

	float Jing = 0.f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float HealthRegenPerSecond = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Energy")
	float EnergyRegenPerSecond = 10.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jing")
	float JingCenteringPerSecond = 5.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jing")
	float JingCenterTolerance = 0.1f;


	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool bWasAttackedRecently = false;
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float RecentlyAttackedTime = 3.f;
	FTimerHandle RecentlyAttackedTimerHandle;

	UPROPERTY(BlueprintReadOnly, Category = "Jing")
	EJingPhase CurrentJingZone = EJingPhase::JP_Neutral;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jing")
	float JingPhaseThreshold = 10.f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// returns Jing value normalized to a 0 to 100 scale
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Jing")
	float GetNormalizedCurrentJing() const;
	// returns Jing value normalized to a 0 to 100 scale
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Jing")
	float GetNormalizedMaxJing() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Energy")
	FORCEINLINE float GetCurrentEnergy() const { return CurrentEnergy; }

	// set corresponding vital and return new value
	UFUNCTION(BlueprintCallable, Category = "Health")
	float UpdateHealth(float HealthDelta);
	// set corresponding vital and return new value
	UFUNCTION(BlueprintCallable, Category = "Energy")
	float UpdateEnergy(float EnergyDelta);
	// set corresponding vital and return new value
	UFUNCTION(BlueprintCallable, Category = "Jing")
	float UpdateJing(float JingDelta);	

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Jing")
	FORCEINLINE EJingPhase GetCurrentJingZone() const { return CurrentJingZone; }


	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	bool bIsEnergyDisabled = false;
};
