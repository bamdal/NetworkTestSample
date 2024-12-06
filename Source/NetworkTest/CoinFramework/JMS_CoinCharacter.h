// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkTest/Charater/JMSCharBase.h"
#include "JMS_CoinCharacter.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKTEST_API AJMS_CoinCharacter : public AJMSCharBase
{
	GENERATED_BODY()
	
public:
	AJMS_CoinCharacter();

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Pickups Cahracter")
	USoundBase* FallSound;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Pickups Cahracter")
	USoundBase* LandSound;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Landed(const FHitResult& Hit) override;

	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

public:
	void AddScore(const float Score) const;
	void AddPickup() const;

	// RPC (Remote Procedure Call)
	UFUNCTION()
	void ClientPlaySound2D(USoundBase* Sound);
};
