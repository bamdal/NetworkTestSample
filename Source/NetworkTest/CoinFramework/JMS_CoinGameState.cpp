// Fill out your copyright notice in the Description page of Project Settings.


#include "JMS_CoinGameState.h"

#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "NetworkTest/Coin/JMS_CoinItem.h"




void AJMS_CoinGameState::BeginPlay()
{
	Super::BeginPlay();

	// 레벨에 있는 모든 ACoinItem 객체를 가져와 담는다.
	TArray<AActor*> CoinItemsArr;
	UGameplayStatics::GetAllActorsOfClass(this, AJMS_CoinItem::StaticClass(), CoinItemsArr);

	PickupsRemaining = CoinItemsArr.Num();
}

TArray<APlayerState*> AJMS_CoinGameState::GetPlayerStatesOrderedByScore() const
{
	// GameStateBase에 PlayerArray를 가져와 캐릭터들을 가져옴
	TArray<APlayerState*> PlayerStates(PlayerArray);
	PlayerStates.Sort([] (const APlayerState& A, const APlayerState& B)
	{
		return A.GetScore() < B.GetScore();
	});
	return PlayerStates;
}

void AJMS_CoinGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AJMS_CoinGameState,PickupsRemaining);
}
