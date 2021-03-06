// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnableAttack.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/BoxComponent.h"
#include "../AvatarWoCtCCharacter.h"
#include "TimerManager.h"
#include "GameFramework/ProjectileMovementComponent.h"

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
	CollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->bGenerateOverlapEvents = true;

	CollisionComp->OnComponentHit.AddDynamic(this, &ASpawnableAttack::AttackHit);

	AttackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AttackMesh"));
	AttackMesh->SetupAttachment(RootComponent);
	AttackMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackMesh->bGenerateOverlapEvents = false;

	RotationComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotationComp"));
	RotationComp->RotationRate = FRotator::ZeroRotator;

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileComp->InitialSpeed = 0.f;
	ProjectileComp->MaxSpeed = 0.f;
}

// Called when the game starts or when spawned
void ASpawnableAttack::BeginPlay()
{
	Super::BeginPlay();

	if (Lifetime > 0.f) GetWorldTimerManager().SetTimer(LifetimeTimerHandle, this, &ASpawnableAttack::DestroySelf, Lifetime); 
}

void ASpawnableAttack::AttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	ASpawnableAttack* Attack = Cast<ASpawnableAttack>(OtherActor);

	EAttackType attack_type = EAttackType::AT_None;
	float attack_damage = 0.f;

	if (Attack)
	{
		attack_type = Attack->Type;
		attack_damage = Attack->Damage;
	}
	else if (bHitByMeleeTrace)
	{
		attack_type = HitType;
		attack_damage = HitDamage;
	}

	if (Attack || bHitByMeleeTrace)
	{
		if (attack_type == this->Type)
		{
			UE_LOG(LogTemp, Warning, TEXT("this attack canceled out another"));
			Destroy();
		}
		else if (attack_damage >= this->Damage)
		{
			UE_LOG(LogTemp, Warning, TEXT("this attack was weaker than the other"));
			Destroy();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("this attack was stronger than the other"));
			Damage = Damage - attack_damage;
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

void ASpawnableAttack::DestroySelf()
{
	Destroy();
}

// Called every frame
void ASpawnableAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnableAttack::AttackHitByMelee(AAvatarWoCtCCharacter* AttackingCharacter, EAttackType MeleeType, float MeleeDamage)
{
	bHitByMeleeTrace = true;
	HitType = MeleeType;
	HitDamage = MeleeDamage;

	FHitResult hit;
	AttackHit(nullptr, AttackingCharacter, nullptr, FVector::ZeroVector, hit);
}

