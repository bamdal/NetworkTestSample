// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetworkTestCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "NetworkTest/ETC/JMSDebugMecros.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ANetworkTestCharacter

ANetworkTestCharacter::ANetworkTestCharacter()
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

void ANetworkTestCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void ANetworkTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FString LocalRoleString = ROLE_TO_STRING(GetLocalRole());
	const FString RemoteRoleString = ROLE_TO_STRING(GetRemoteRole());

	const AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	const AGameStateBase* GameState = GetWorld()->GetGameState();
	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	const AHUD* HUD = PlayerController != nullptr ? PlayerController->GetHUD() : nullptr;

	const FString GameModeString = GameMode != nullptr ? TEXT("Valid GameMode") : TEXT("Invalid GameMode");
	const FString GameStateString = GameState != nullptr ? TEXT("Valid GameState") : TEXT("Invalid GameState");
	const FString PlayerStateString = GetPlayerState() != nullptr ? TEXT("Valid PlayerState") : TEXT("Invalid PlayerState");
	const FString PawnName = GetName();
	const FString PlayerControllerString = PlayerController != nullptr ? TEXT("Valid PlayerController") : TEXT("Invalid PlayerController");
	const FString HUDString = HUD != nullptr ? TEXT("Valid HUD") : TEXT("Invalid HUD");
	
	const FString Values = FString::Printf(TEXT("LocalRole: %s\nRemoteRole = %s\nGameMode = %s\nGameState = %s\nPlayerState = %s\nPawnName = %s\nPlayerController = %s\nHUD = %s\n")
		, *LocalRoleString, *RemoteRoleString, *GameModeString,*GameStateString,*PlayerStateString,*PawnName, *PlayerControllerString, *HUDString);
	DrawDebugString(GetWorld(),GetActorLocation(),Values,nullptr,FColor::White,0.0f,true);
}


//////////////////////////////////////////////////////////////////////////
// Input

void ANetworkTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 나중에 추가될 입력 처리
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}



	void ANetworkTestCharacter::OpenLevel()
	{
		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			World->ServerTravel("/Game/Maps/L_Lobby?listen");
		}
	}

	// 그냥 씬이동 간소화
	void ANetworkTestCharacter::CallOpenLevel(const FString& Address)
	{
		UGameplayStatics::OpenLevel(this,*Address);
	}

	void ANetworkTestCharacter::CallClientTravel(const FString& Address)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetGameInstance()->GetFirstLocalPlayerController());
		if(PlayerController)
		{
			PlayerController->ClientTravel(Address,TRAVEL_Absolute);
		}
	}

