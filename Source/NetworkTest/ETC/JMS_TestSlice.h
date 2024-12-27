// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JMS_TestSlice.generated.h"

UCLASS()
class NETWORKTEST_API AJMS_TestSlice : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJMS_TestSlice();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
