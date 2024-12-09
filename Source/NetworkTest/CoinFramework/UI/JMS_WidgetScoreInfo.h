// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JMS_WidgetScoreInfo.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKTEST_API UJMS_WidgetScoreInfo : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	TObjectPtr<class AJMS_CoinPlayerState> CoinPlayerState;

	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<class UTextBlock> T_PlayerName;
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<class UTextBlock> T_Score;
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<class UTextBlock> T_Coin;

public:
	void SetTextBox(FSlateColor& SColor);
};
