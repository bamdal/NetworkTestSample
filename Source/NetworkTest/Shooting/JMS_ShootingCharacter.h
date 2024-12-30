// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "ProceduralMeshComponent.h"
#include "NetworkTest/Charater/JMSCharBase.h"
#include "JMS_ShootingCharacter.generated.h"

/**
 * 
 */
UCLASS(config=Game)
class NETWORKTEST_API AJMS_ShootingCharacter : public AJMSCharBase
{
	GENERATED_BODY()
public:

	AJMS_ShootingCharacter();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input",meta=(AllowPrivateAccess = "true"))
	UInputAction* IA_Shoot;

protected:

	// 최대체력
	UPROPERTY(EditDefaultsOnly,Config,Category="Status")
	float MaxHP = 0;

	// Rep_ 현재체력 
	UPROPERTY(EditDefaultsOnly,ReplicatedUsing = OnRep_CurrentHP,Category="Status")
	float CurrentHP;

	// 총알 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<class AJMS_Projectile> ProjectileClass;

	// 발사 딜레이
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float FireRate;

	// 발사체 쐈을 때 true 설정
	bool bIsFiringWeapon = false;

	FTimerHandle FiringTimer;

	UPROPERTY()
	FName CutBoneName;
	
protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	
	UFUNCTION()
	void Shoot(const FInputActionInstance& InputActionInstance);

	UFUNCTION()
	void OnRep_CurrentHP();

	UFUNCTION()
	void OnHealthUpdate();

public:

	// 체력 -------------------------------------------------
	
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable,Category="Status")
	void SetCurrentHealth(float Hp);

	UFUNCTION(BlueprintPure,Category="Status")
	float GetMaxHealth() const { return MaxHP; };
	
	UFUNCTION(BlueprintPure,Category="Status")
	float GetCurrentHealth() const { return CurrentHP; };

	// 총 발사 관리 -----------------------------------------------------
	
	UFUNCTION(BlueprintCallable,Category="Gameplay")
	void StartFire();

	UFUNCTION(BlueprintCallable,Category="Gameplay")
	void StopFire();

	UFUNCTION()
	UProceduralMeshComponent* CreateProceduralMeshFromBone(USkeletalMeshComponent* SkeletalMesh, const FName& BoneName);

	UFUNCTION()
	void AttachMeshToBone(UProceduralMeshComponent* MeshPart, const FName& BoneName);

	UFUNCTION(Server,Reliable)
	void HandleFire();

	UFUNCTION(Server,Reliable)
	void ServerHandleCutting();
	
	UFUNCTION(NetMulticast,Reliable)
	void StartCutting();


public:
	UFUNCTION(BlueprintCallable)
	void ApplySliceEffect(const FName& BoneName);
};





