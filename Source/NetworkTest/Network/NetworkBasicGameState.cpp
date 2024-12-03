// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkBasicGameState.h"

#include "NetworkTest/ETC/JMSDebugMecros.h"


void ANetworkBasicGameState::HandleBeginPlay()
{
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("Begin"));
	Super::HandleBeginPlay();
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("End"));
	
}

void ANetworkBasicGameState::OnRep_ReplicatedHasBegunPlay()
{
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("Begin"));
	Super::OnRep_ReplicatedHasBegunPlay();
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("End"));

}
