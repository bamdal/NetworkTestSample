// Fill out your copyright notice in the Description page of Project Settings.


#include "JMS_WidgetScoreBoard.h"

#include "JMS_WidgetScoreInfo.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "NetworkTest/CoinFramework/JMS_CoinGameState.h"
#include "NetworkTest/CoinFramework/JMS_CoinPlayerController.h"
#include "NetworkTest/CoinFramework/JMS_CoinPlayerState.h"

void UJMS_WidgetScoreBoard::NativeConstruct()
{
	Super::NativeConstruct();

	AJMS_CoinGameState* GameState = Cast<AJMS_CoinGameState>(GetWorld()->GetGameState());
	if (GameState != nullptr)
	{
		CoinGameState = GameState;
	}
	UpdateScoreBoard();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UJMS_WidgetScoreBoard::UpdateScoreBoard, 0.5f, true);
}

void UJMS_WidgetScoreBoard::NativeDestruct()
{
	Super::NativeDestruct();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UJMS_WidgetScoreBoard::UpdateScoreBoard()
{
	if (CoinGameState == nullptr)
	{
		return;
	}
	// 남은 코인 카운트
	FString str = FString::Printf(TEXT("[%d] Pickup(s) Remaining"), CoinGameState->GetPickupRemaining());

	T_PickupRemaining->SetText(FText::FromString(str));

	V_PlayerCoinBoard->ClearChildren();

	UJMS_WidgetScoreInfo* Header = CreateWidget<UJMS_WidgetScoreInfo>(this, UJMS_WidgetScoreInfo::StaticClass());
	if (Header != nullptr)
	{
		FSlateColor SC(FColor::Green);
		Header->SetTextBox(SC);
		V_PlayerCoinBoard->AddChild(Header);
	}

	TArray<APlayerState*> Players = CoinGameState->GetPlayerStatesOrderedByScore();
	for (APlayerState* State : Players)
	{
		AJMS_CoinPlayerState* CoinPlayerState = Cast<AJMS_CoinPlayerState>(State);
		AJMS_CoinPlayerController* Controller = Cast<AJMS_CoinPlayerController>(UGameplayStatics::GetPlayerController(this,0));
		if (CoinPlayerState != nullptr)
		{
			/*UClass* WidgetBlueprintClass = LoadClass<UUserWidget>(
				nullptr,
				TEXT("/Game/BP/Coin/WBP_ScoreBoard_Info1.WBP_ScoreBoard_Info1_C") // _C 추가
			);*/
			UJMS_WidgetScoreInfo* Entry = nullptr;
			if(Controller)
				Entry = CreateWidget<UJMS_WidgetScoreInfo>(this,Controller->WidgetScoreInfo);
			if (Entry)
			{
				Entry->CoinPlayerState = CoinPlayerState;
				// 본인의 정보인지 판별
				if (CoinPlayerState->GetPawn() == Entry->GetOwningPlayerPawn())
				{
					FSlateColor SC(FColor::Blue);
					Entry->SetTextBox(SC);
				}
				else
				{
					FSlateColor SC(FColor::White);
					Entry->SetTextBox(SC);
				}

				V_PlayerCoinBoard->AddChild(Entry);
			}
		}
	}
}
