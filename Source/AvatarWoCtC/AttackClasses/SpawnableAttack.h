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
	
public:	
	// Sets default values for this actor's properties
	ASpawnableAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttackInfo")
		EConditionEffect CondEffect = EConditionEffect::CE_None;
	*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	EAttackType Type = EAttackType::AT_None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float Damage = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* AttackMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	class URotatingMovementComponent* RotationComp;
};
