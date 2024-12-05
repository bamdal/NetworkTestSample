// Fill out your copyright notice in the Description page of Project Settings.


#include "JMS_CoinPlayerState.h"

#include "Net/UnrealNetwork.h"

void AJMS_CoinPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AJMS_CoinPlayerState,Pickups);
}
