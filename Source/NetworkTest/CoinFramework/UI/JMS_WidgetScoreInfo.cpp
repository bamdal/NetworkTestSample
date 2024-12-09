// Fill out your copyright notice in the Description page of Project Settings.


#include "JMS_WidgetScoreInfo.h"

#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "NetworkTest/CoinFramework/JMS_CoinPlayerState.h"


void UJMS_WidgetScoreInfo::NativeConstruct()
{
	Super::NativeConstruct();


}

void UJMS_WidgetScoreInfo::NativeDestruct()
{
	Super::NativeDestruct();
}

void UJMS_WidgetScoreInfo::SetTextBox(FSlateColor& SColor)
{
	
	if (T_PlayerName && CoinPlayerState)
	{
		T_PlayerName->SetColorAndOpacity(SColor);
		const FString str = FString::Printf(TEXT("%s"), *CoinPlayerState->GetPlayerName());
		T_PlayerName->SetText(FText::FromString(*str));
	}
	if (T_Score && CoinPlayerState)
	{
		T_Score->SetColorAndOpacity(SColor);
		const FString str = FString::Printf(TEXT("%d"), static_cast<int32>(CoinPlayerState->GetScore()));
		T_Score->SetText(FText::FromString(*str));
	}
	if (T_Coin && CoinPlayerState)
	{
		T_Coin->SetColorAndOpacity(SColor);
		const FString str = FString::Printf(TEXT("%d"), CoinPlayerState->GetPickups());
		T_Coin->SetText(FText::FromString(*str));
	}
}
