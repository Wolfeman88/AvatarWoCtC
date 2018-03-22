// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "MeleeAttackComponent.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	AT_None			UMETA(Display = "None"),
	AT_Light		UMETA(DisplayName = "LightAttack"),
	AT_Heavy		UMETA(DisplayName = "HeavyAttack"),
	AT_Stun			UMETA(DisplayName = "StunAttack")
};

USTRUCT(BlueprintType)
struct FMeleeTrace : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY()
	FVector TraceSource;

	UPROPERTY()
	FVector TraceEnd;
};

USTRUCT(BlueprintType)
struct FMeleeAttack : public FTableRowBase
{
	GENERATED_BODY()

	// sets of line trace start and end vectors that represent up the attack
	UPROPERTY()
	TArray<FMeleeTrace> Traces;

	UPROPERTY()
	float Damage = 0.f;

	// EConditionEffect Condition = Knockback, Stagger, Stun, Doused, Knockdown, Frozen, etc.
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AVATARWOCTC_API UMeleeAttackComponent : public UActorComponent
{
	GENERATED_BODY()

	TArray<AActor*> ActorsHitByAttack;

	EAttackType QueuedAttack = EAttackType::AT_None;
	FMeleeAttack QueuedAttackData;

	EAttackType CurrentAttack = EAttackType::AT_None;
	FMeleeAttack CurrentAttackData;

	FVector GetRelativeVectorOffset(FVector Offset);
	void EndAttack();

public:	
	// Sets default values for this component's properties
	UMeleeAttackComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Melee")
	void ActivateMeleeAbility(EAttackType NewAttack, FMeleeAttack AttackData);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timer")
	float LightAttackActivationTime = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timer")
	float HeavyAttackActivationTime = 2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timer")
	float StunAttackActivationTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed")
	float LightAttackSpeedFactor = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed")
	float HeavyAttackSpeedFactor = 0.25f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed")
	float StunAttackSpeedFactor = 0.75f;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Attack Timer")
	FTimerHandle AttackTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Range")
	float Close = 73.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Range")
	float Middle = 146.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Range")
	float Max = 219.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Width")
	float Inner = 44.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Width")
	float Outer = 88.f;

	void Light();
	void Heavy();
	void Stun();
};
