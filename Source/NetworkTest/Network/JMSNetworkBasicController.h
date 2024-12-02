// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JMSNetworkBasicController.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKTEST_API AJMSNetworkBasicController : public APlayerController
{
	GENERATED_BODY()
	
	
	virtual void BeginPlay() override;
	
};
