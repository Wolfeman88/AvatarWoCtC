// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpecialAbilityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AVATARWOCTC_API USpecialAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpecialAbilityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Special")
	bool bIsSpecialActive = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo")
	bool bIsUsingCombo = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combo")
	int32 ComboCount = 0;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Special")
	virtual void ActivateSpecial();
	UFUNCTION(BlueprintCallable, Category = "Special")
	virtual void DeactivateSpecial();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Special")
	FORCEINLINE bool GetIsSpecialActive() const { return bIsSpecialActive; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combo")
	bool GetIsUsingCombo() const { return bIsUsingCombo; }

	UFUNCTION(BlueprintCallable, Category = "Combo")
	void ResetCombo();
	UFUNCTION(BlueprintCallable, Category = "Combo")
	void IncrementCombo();
};
