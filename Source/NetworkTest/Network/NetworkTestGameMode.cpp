// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetworkTestGameMode.h"

#include "../ETC/JMSDebugMecros.h"

ANetworkTestGameMode::ANetworkTestGameMode()
{

}

void ANetworkTestGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("Begin ===== Client Login ===="));
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("End ===== Client Login ===="));
}

APlayerController* ANetworkTestGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
	const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("Begin"));
	APlayerController* NewPlayerController =  Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("End"));
	return NewPlayerController;
}

void ANetworkTestGameMode::PostLogin(APlayerController* NewPlayer)
{
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("Begin"));
	Super::PostLogin(NewPlayer);
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("End"));

}

void ANetworkTestGameMode::StartPlay()
{
	Super::StartPlay();
}
