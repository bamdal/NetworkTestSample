// Fill out your copyright notice in the Description page of Project Settings.


#include "JMS_CoinGameMode.h"

#include "JMS_CoinCharacter.h"
#include "JMS_CoinGameState.h"
#include "Kismet/GameplayStatics.h"
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

// 게임 종료시 호출
void AJMS_CoinGameMode::HandleMatchHasEnded()
{
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("Begin"));
	Super::HandleMatchHasEnded();
	TArray<AActor*> Characters;
	UGameplayStatics::GetAllActorsOfClass(this,AJMS_CoinCharacter::StaticClass(),Characters);
	for (AActor* Character : Characters)
	{
		Character->Destroy();
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle,this, &AJMS_CoinGameMode::RestartMap,5.0f);
	
	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,"JMS_CoinGameMode::HandleMatchHasEnded");
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("End"));
}

bool AJMS_CoinGameMode::ReadyToStartMatch_Implementation()
{
	return true;
}

bool AJMS_CoinGameMode::ReadyToEndMatch_Implementation()
{
	return CoinGameState!=nullptr && !CoinGameState->HasPickup();
}

void AJMS_CoinGameMode::RestartMap() const
{
	GetWorld()->ServerTravel(GetWorld()->GetName());
}
