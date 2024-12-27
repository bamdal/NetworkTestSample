// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JMS_Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class NETWORKTEST_API AJMS_Projectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AJMS_Projectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MovementComp;

	// 충돌 폭발 파티클
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	UParticleSystem* ExplosionEffect;

	// 충돌시 데미지 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;

	// 충돌 데미지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float Damage;


	UFUNCTION()
	void OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                        FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
