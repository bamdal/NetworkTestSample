// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "JMS_CoinGameMode.generated.h"

class AJMS_CoinPlayerState;
class AJMS_CoinGameState;
/**
 * 
 */
//UCLASS(config = CoinGameRule)
UCLASS(config = Game)
class NETWORKTEST_API AJMS_CoinGameMode : public AGameMode
{
	GENERATED_BODY()

	AJMS_CoinGameMode();

protected:
	UPROPERTY()
	AJMS_CoinGameState* CoinGameState;

public:
	UPROPERTY(Config)
	int32 PickupPoint;

	UPROPERTY(Config)
	int32 FellOutPoint;

protected:
	virtual void BeginPlay() override;

	virtual bool ShouldSpawnAtStartSpot(AController* Player);

	// 상태가 InProgress로 전환될 때 호출됩니다.
	virtual void HandleMatchHasStarted() override;
	
	// 맵이 WaitingPostMatch로 전환될 때 호출됩니다
	virtual void HandleMatchHasEnded() override;

	virtual bool ReadyToStartMatch_Implementation() override;

	virtual bool ReadyToEndMatch_Implementation() override;
	
	void RestartMap() const;
};
