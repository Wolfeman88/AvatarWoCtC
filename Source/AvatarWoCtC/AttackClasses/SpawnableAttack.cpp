// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnableAttack.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/BoxComponent.h"
#include "../AvatarWoCtCCharacter.h"

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

	CollisionComp->OnComponentHit.AddDynamic(this, &ASpawnableAttack::AttackHit);

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

void ASpawnableAttack::AttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	ASpawnableAttack* Attack = Cast<ASpawnableAttack>(OtherActor);

	if (Attack)
	{
		if (Attack->Type == this->Type)
		{
			UE_LOG(LogTemp, Warning, TEXT("this attack canceled out another"));
			Destroy();
		}
		else if (Attack->Damage >= this->Damage)
		{
			UE_LOG(LogTemp, Warning, TEXT("this attack was weaker than the other"));
			Destroy();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("this attack was stronger than the other"));
		}
	}
	else
	{
		AAvatarWoCtCCharacter* Character = Cast<AAvatarWoCtCCharacter>(OtherActor);

		if (Character)
		{
			UE_LOG(LogTemp, Warning, TEXT("this attack hit a character"));
		}
	}
}

// Called every frame
void ASpawnableAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

