// Fill out your copyright notice in the Description page of Project Settings.


#include "JMSNetworkBasicController.h"

#include "NetworkTest/ETC/JMSDebugMecros.h"


void AJMSNetworkBasicController::BeginPlay()
{
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("Begin "));
	Super::BeginPlay();
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("End"));
}

void AJMSNetworkBasicController::OnPossess(APawn* InPawn)
{
	
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("Begin "));
	Super::OnPossess(InPawn);
	JMSLOG_NET_LOG(Log,TEXT("%s"),TEXT("End"));
}
