// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "JMSCharBase.h"
#include "Logging/LogMacros.h"
#include "NetworkTestCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ANetworkTestCharacter : public AJMSCharBase
{
	GENERATED_BODY()


public:
	ANetworkTestCharacter();
	


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();




public:
	UFUNCTION(BlueprintCallable)
	void OpenLevel();

	UFUNCTION(BlueprintCallable)
	void  CallOpenLevel(const FString& Address);
	
	UFUNCTION(BlueprintCallable)
	void CallClientTravel(const FString& Address);
};

