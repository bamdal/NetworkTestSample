// Fill out your copyright notice in the Description page of Project Settings.


#include "JMS_CoinGameMode.h"
#include "JMS_CoinGameState.h"
#include "NetworkTest/ETC/JMSDebugMecros.h"

void AJMS_CoinGameMode::BeginPlay()
{
	Super::BeginPlay();

	CoinGameState = GetGameState<AJMS_CoinGameState>();
}

// 게임이 처음 시작되었을떄
void AJMS_CoinGameMode::HandleMatchHasStarted()
{
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("Begin"));
	Super::HandleMatchHasStarted();

	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,"JMS_CoinGameMode::HandleMatchHasStarted");
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("End"));
}

void AJMS_CoinGameMode::HandleMatchHasEnded()
{
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("Begin"));
	Super::HandleMatchHasEnded();

	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,"JMS_CoinGameMode::HandleMatchHasEnded");
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("End"));
}
