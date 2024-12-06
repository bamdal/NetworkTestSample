// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JMS_CoinPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKTEST_API AJMS_CoinPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Score")
	TSubclassOf<UUserWidget> ScoreboardMenuClass;

	UPROPERTY()
	class UUserWidget* ScoreboardMenu;

protected:
	virtual void BeginPlay() override;
};
