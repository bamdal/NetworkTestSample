// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "NetworkTest/Charater/JMSCharBase.h"
#include "JMS_ShootingCharacter.generated.h"

/**
 * 
 */
UCLASS(config=Game)
class NETWORKTEST_API AJMS_ShootingCharacter : public AJMSCharBase
{
	GENERATED_BODY()

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
	
public:

	AJMS_ShootingCharacter();
	
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
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable,Category="Status")
	void SetCurrentHealth(float Hp);

	UFUNCTION(BlueprintPure,Category="Status")
	float GetMaxHealth() const { return MaxHP; };
	
	UFUNCTION(BlueprintPure,Category="Status")
	float GetCurrentHealth() const { return CurrentHP; };
	
	
};



