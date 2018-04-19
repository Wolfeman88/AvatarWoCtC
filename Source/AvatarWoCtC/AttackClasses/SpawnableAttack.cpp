// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnableAttack.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ASpawnableAttack::ASpawnableAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//BaseNode = CreateDefaultSubobject<USceneComponent>(TEXT("BaseNode"));
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;
	CollisionComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->bGenerateOverlapEvents = true;

	AttackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AttackMesh"));
	AttackMesh->SetupAttachment(RootComponent);
	AttackMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackMesh->bGenerateOverlapEvents = false;

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

