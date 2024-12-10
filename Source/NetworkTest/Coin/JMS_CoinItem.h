// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JMS_CoinItem.generated.h"

class URotatingMovementComponent;

UCLASS()
class NETWORKTEST_API AJMS_CoinItem : public AActor
{
	GENERATED_BODY()

protected:

	// 아이템 매시
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Item")
	UStaticMeshComponent* Mesh;

	// 아이템 회전
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Item")
	URotatingMovementComponent* RotatingMovementComponent;

	// 아이템 획득 사운드
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Item")
	USoundBase* PickupSound;

private:
	UPROPERTY(EditAnywhere,Category="Item")
	bool ItemTypeSpeed = false;
	
	UPROPERTY(EditAnywhere)
	float BaseSpeedBuff = 1000.0f;

	UPROPERTY(EditAnywhere)
	float SpeedBuffTime = 2.0f;
public:	
	// Sets default values for this actor's properties
	AJMS_CoinItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
