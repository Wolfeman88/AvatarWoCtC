// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../AttackComponent.h"
#include "SpawnableAttack.generated.h"

UCLASS()
class AVATARWOCTC_API ASpawnableAttack : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* AttackMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	class URotatingMovementComponent* RotationComp;

	bool bHitByMeleeTrace = false;
	EAttackType HitType = EAttackType::AT_None;
	float HitDamage = 0.f;

	FTimerHandle LifetimeTimerHandle;
	
public:	
	// Sets default values for this actor's properties
	ASpawnableAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void AttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, Category = "Life")
	void DestroySelf();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	EAttackType Type = EAttackType::AT_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float Damage = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Life")
	float Lifetime = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Alternate")
	TSubclassOf<ASpawnableAttack> AlternateAttack = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Category = "Attack")
	virtual void AttackHitByMelee(AAvatarWoCtCCharacter* AttackingCharacter, EAttackType MeleeType, float MeleeDamage);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attack")
	FORCEINLINE EAttackType GetType() const { return Type; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attack")
	FORCEINLINE float GetDamage() const { return Damage; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Alternate")
	FORCEINLINE TSubclassOf<ASpawnableAttack> GetAlternateAttack() const { return AlternateAttack; }
};
