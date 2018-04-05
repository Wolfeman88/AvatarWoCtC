// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnableAttack.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"

// Sets default values
ASpawnableAttack::ASpawnableAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AttackMesh"));
	RootComponent = AttackMesh;

	RotationComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotationComp"));
	RotationComp->RotationRate = FRotator::ZeroRotator;
}

// Called when the game starts or when spawned
void ASpawnableAttack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnableAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

