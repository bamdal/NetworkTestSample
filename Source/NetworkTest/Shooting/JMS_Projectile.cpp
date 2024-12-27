// Fill out your copyright notice in the Description page of Project Settings.


#include "JMS_Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AJMS_Projectile::AJMS_Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 날아가는게 보이기 위해 리플리케이트 활성화
	bReplicates = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
	SphereComp->InitSphereRadius(37.5f);
	SphereComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(SphereComp);
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// UStaticMesh 에셋 직접 경로 찾아 가져오기
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(
		TEXT("/Game/DUWepCustSys/SM_RocketStudio.SM_RocketStudio"));
	if (DefaultMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(DefaultMesh.Object);
		MeshComp->SetRelativeLocation(FVector(0, 0, -37.5f));
		MeshComp->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));
	}

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->SetUpdatedComponent(SphereComp);
	MovementComp->InitialSpeed = 1500.0f;
	MovementComp->MaxSpeed = 1500.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->ProjectileGravityScale = 0.0f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DefaultExplosionEffect(
		TEXT("/Game/Shooting/FX/Particles/P_Explosion.P_Explosion"));
	if (DefaultExplosionEffect.Succeeded())
	{
		ExplosionEffect = DefaultExplosionEffect.Object; // 컴파일 오류 없음
	}

	DamageType = UDamageType::StaticClass();
	Damage = 10.0f;

	// 서버만 충돌 판정 바인딩
	if (HasAuthority())
	{
		SphereComp->OnComponentHit.AddDynamic(this,&ThisClass::OnProjectileImpact);
	}
}


void AJMS_Projectile::OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
							FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		UGameplayStatics::ApplyPointDamage(OtherActor,Damage,NormalImpulse,Hit,GetInstigator()->GetController(),this,DamageType);
	}

	Destroy();
}

// Called when the game starts or when spawned
void AJMS_Projectile::BeginPlay()
{
	Super::BeginPlay();
}

void AJMS_Projectile::Destroyed()
{
	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, GetActorLocation(), FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
	}

	Super::Destroyed();
}

// Called every frame
void AJMS_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
