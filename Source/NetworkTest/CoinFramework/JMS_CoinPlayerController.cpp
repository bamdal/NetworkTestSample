// Fill out your copyright notice in the Description page of Project Settings.


#include "JMS_CoinPlayerController.h"

#include "Blueprint/UserWidget.h"


void AJMS_CoinPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 로컬서버인지 확인은 데디서버가 아님을 의미
	if(IsLocalController() && ScoreboardMenuClass != nullptr)
	{
		ScoreboardMenu = CreateWidget<UUserWidget>(this, ScoreboardMenuClass);
		if(ScoreboardMenu)
		{
			ScoreboardMenu->AddToViewport();
		}
	}
	
}
