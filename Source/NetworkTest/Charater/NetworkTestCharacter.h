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
	

	virtual void Tick(float DeltaTime) override; 

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();


	virtual void PossessedBy(AController* NewController) override;


public:

	
	UFUNCTION(BlueprintCallable)
	void OpenLevel();

	UFUNCTION(BlueprintCallable)
	void  CallOpenLevel(const FString& Address);
	
	UFUNCTION(BlueprintCallable)
	void CallClientTravel(const FString& Address);

protected:
	// 이 변수가 복제되어 갈것이라 알림
	UPROPERTY(Replicated)
	float R_Health = 100.0f;

	UPROPERTY(ReplicatedUsing= OnRep_Mana)
	int32 RU_Mana;

	UFUNCTION()
	void OnRep_Mana();
};

