// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "JMS_CoinGameState.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKTEST_API AJMS_CoinGameState : public AGameState
{
	GENERATED_BODY()

protected:
	//레벨에 남아있는 전체 쾬수
	UPROPERTY(Replicated,BlueprintReadOnly)
	int32 PickupsRemaining;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	TArray<APlayerState*> GetPlayerStatesOrderedByScore() const;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void RemovePickup() {PickupsRemaining--;}
    bool HasPickup() const {return PickupsRemaining>0;};

};
