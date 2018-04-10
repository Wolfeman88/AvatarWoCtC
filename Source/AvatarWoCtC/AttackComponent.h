// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "AttackComponent.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	AT_None			UMETA(Display = "None"),
	AT_Light		UMETA(DisplayName = "LightAttack"),
	AT_Heavy		UMETA(DisplayName = "HeavyAttack"),
	AT_Stun			UMETA(DisplayName = "StunAttack")
};

UENUM(BlueprintType)
enum class EDistancePoints : uint8
{
	DP_None = 0		UMETA(Display = "No Offset"),
	DP_Close = 73	UMETA(Display = "Close"),
	DP_Middle = 146	UMETA(Display = "Middle"),
	DP_Far = 219	UMETA(Display = "Far")
};

UENUM(BlueprintType)
enum class EWidthPoints : uint8
{
	DP_Outer_Neg = 0	UMETA(Display = "Negative Outer"),
	DP_Inner_Neg = 44	UMETA(Display = "Negative Inner"),
	DP_None = 88		UMETA(Display = "No Offset"),
	DP_Inner_Pos = 132	UMETA(Display = "Positive Inner"),
	DP_Outer_Pos = 176	UMETA(Display = "Positive Outer")
};

USTRUCT(BlueprintType)
struct FMeleeVector : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Attack Data")
	EDistancePoints ForwardOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Attack Data")
	EWidthPoints RightOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Attack Data")
	EWidthPoints UpOffset;
};

USTRUCT(BlueprintType)
struct FMeleeTrace : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Attack Data")
	FMeleeVector Source;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Attack Data")
	FMeleeVector End;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Attack Data")
	TArray<FMeleeTrace> Traces;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Attack Data")
	float Damage = 0.f;

	// EConditionEffect Condition = Knockback, Stagger, Stun, Doused, Knockdown, Frozen, etc.
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AVATARWOCTC_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

	TArray<AActor*> ActorsHitByAttack;

	EAttackType M_QueuedAttack = EAttackType::AT_None;
	FMeleeAttack M_QueuedAttackData;
	EAttackType M_CurrentAttack = EAttackType::AT_None;
	FMeleeAttack M_CurrentAttackData;

	FVector ConvertMeleeVector(FMeleeVector EnumOffset);
	FVector GetRelativeVectorOffset(FVector Offset);
	void EndAttack();
	void ClearQueue();
	
	class AAvatarWoCtCCharacter* OwningCharacter = nullptr;

	TSubclassOf<AActor> R_QueuedAttack = nullptr;
	FVector R_QueuedOffset = FVector::ZeroVector;
	TSubclassOf<AActor> R_CurrentAttack = nullptr;
	FVector R_CurrentOffset = FVector::ZeroVector;

	TSubclassOf<AActor> DM_QueuedAttack = nullptr;
	float DM_QueuedOffset = 0.f;
	TSubclassOf<AActor> DM_CurrentAttack = nullptr;
	float DM_CurrentOffset = 0.f;

	TSubclassOf<AActor> DR_QueuedAttack = nullptr;
	float DR_QueuedOffset = 0.f;
	TSubclassOf<AActor> DR_CurrentAttack = nullptr;
	float DR_CurrentOffset = 0.f;

public:	
	// Sets default values for this component's properties
	UAttackComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Melee")
	void ActivateMeleeAbility(EAttackType NewAttack, FMeleeAttack AttackData);
	UFUNCTION(BlueprintCallable, Category = "Ranged")
	void ActivateRangedAbility(TSubclassOf<AActor> AttackToSpawn, FVector OriginationOffset);
	UFUNCTION(BlueprintCallable, Category = "Defense - Melee")
	void ActivateMeleeDefenseAbility(TSubclassOf<AActor> AttackToSpawn, float OriginationOffset);
	UFUNCTION(BlueprintCallable, Category = "Defense - Ranged")
	void ActivateRangedDefenseAbility(TSubclassOf<AActor> AttackToSpawn, float OriginationOffset);

	UFUNCTION(BlueprintCallable, Category = "Melee")
	FMeleeAttack GetRandomAttackData(EAttackType AttackStyle);

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

	bool GetAttackTimerActive() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed")
	float LightAbilityJingDelta = 5.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed")
	float HeavyAbilityJingDelta = 12.5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed")
	float StunAbilityJingDelta = 2.f;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Attack Timer")
	FTimerHandle AttackTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee Attack Data")
	class UDataTable* LightAttackData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee Attack Data")
	UDataTable* HeavyAttackData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee Attack Data")
	UDataTable* StunAttackData;

	void M_Light();
	void M_Heavy();
	void M_Stun();

	void R_Attack();

	void M_Defense();

	void R_Defense();
};
