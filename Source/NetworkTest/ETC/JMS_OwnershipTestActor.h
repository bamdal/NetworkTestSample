// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JMS_OwnershipTestActor.generated.h"

UCLASS()
class NETWORKTEST_API AJMS_OwnershipTestActor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VIsibleAnywhere, BlueprintReadOnly, Category = "Test")
	class UStaticMeshComponent* Mesh;

	// 4미터내에 접근하면 Owner작성
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	float OwnershipRadius = 400.0f; 
	
public:	
	// Sets default values for this actor's properties
	AJMS_OwnershipTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
