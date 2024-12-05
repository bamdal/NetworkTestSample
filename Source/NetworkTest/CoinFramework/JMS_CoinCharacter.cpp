// Fill out your copyright notice in the Description page of Project Settings.


#include "JMS_CoinCharacter.h"

#include "JMS_CoinPlayerState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/DialogueWave.h"


AJMS_CoinCharacter::AJMS_CoinCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
}

void AJMS_CoinCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

void AJMS_CoinCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(EndPlayReason == EEndPlayReason::Destroyed)
	{
		UGameplayStatics::PlaySound2D(GetWorld(),FallSound);
	}
}

void AJMS_CoinCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	UGameplayStatics::PlaySound2D(GetWorld(),LandSound);
}

// 기존 점수에 Score를 시키는 함수
void AJMS_CoinCharacter::AddScore(const float Score) const
{
	APlayerState* MyPlayerState = GetPlayerState();
	if(MyPlayerState != nullptr)
	{
		const float CurrentScore = MyPlayerState->GetScore();
		MyPlayerState->SetScore(CurrentScore + Score);
	}
}

// 플레이어 스테이트의 리플리케이션 호출
void AJMS_CoinCharacter::AddPickup() const
{
	AJMS_CoinPlayerState* MyPlayerState = GetPlayerState<AJMS_CoinPlayerState>();
	if(MyPlayerState != nullptr)
	{
		MyPlayerState->AddPickup();
	}
}

