// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JMS_WidgetScoreBoard.generated.h"

class UJMS_WidgetScoreInfo;
/**
 * 
 */
UCLASS()
class NETWORKTEST_API UJMS_WidgetScoreBoard : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

public:

	UPROPERTY()
	TObjectPtr<class AJMS_CoinGameState> CoinGameState;
	
	
	UPROPERTY()
	FTimerHandle TimerHandle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> T_PickupRemaining;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UVerticalBox> V_PlayerCoinBoard;
	
	UFUNCTION()
	void UpdateScoreBoard();
		
	
	
	
	
};
