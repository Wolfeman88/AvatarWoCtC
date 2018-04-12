// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpecialAbilityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AVATARWOCTC_API USpecialAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

	class AAvatarWoCtCCharacter* OwningCharacter = nullptr;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo")
	int32 ComboTier2Threshold = 3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo")
	int32 ComboTier3Threshold = 6;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo")
	int32 MaxCombo = 10;

	UPROPERTY(BlueprintReadOnly, Category = "Combo")
	int32 ComboCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Special")
	TSubclassOf<class ASpawnableAttack> SpecialWeaponProjectile = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Special")
	TSubclassOf<ASpawnableAttack> MaxCombo_SpecialWeaponProjectile = nullptr;

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
	FORCEINLINE bool GetIsUsingCombo() const { return bIsUsingCombo; }

	UFUNCTION(BlueprintCallable, Category = "Combo")
	void ResetCombo();
	UFUNCTION(BlueprintCallable, Category = "Combo")
	void IncrementCombo();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combo")
	FORCEINLINE bool GetIsComboMax() const { return ComboCount >= MaxCombo; };

	UFUNCTION(BlueprintCallable, Category = "Special")
	virtual void ThrowSpecialProjectile();

	UFUNCTION(BlueprintCallable, Category = "Special")
	void SetReferences(AAvatarWoCtCCharacter* owner, TSubclassOf<ASpawnableAttack> special, TSubclassOf<ASpawnableAttack> max_special);
};
