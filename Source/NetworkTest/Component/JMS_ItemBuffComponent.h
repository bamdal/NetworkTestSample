// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JMS_ItemBuffComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NETWORKTEST_API UJMS_ItemBuffComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class AJMS_CoinCharacter* CoinGameCharacter;

	FTimerHandle SpeedBuffTimerHandle;
	
	//캐릭터의 기본속도
	float InitialBaseSpeed;

	void ResetSpeedBuff();

	UFUNCTION(NetMulticast,Reliable)
	void MulticastSpeedBuff(float BaseSpeed);
public:
	friend class AJMS_CoinCharacter;
	void SetInitialSpeed(float Speed)
	{
		InitialBaseSpeed = Speed;
	};

	void SpeedBuffServerProc(float BuffSpeed,float BuffTime);
	
public:	
	// Sets default values for this component's properties
	UJMS_ItemBuffComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
