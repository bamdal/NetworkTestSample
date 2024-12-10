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
#include "Net/UnrealNetwork.h"
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

	FString Values = FString::Printf(
		TEXT("LocalRole: %s\nRemoteRole = %s\nGameMode = %s\nGameState = %s\nPlayerState = %s\nPawnName = %s\nPlayerController = %s\nHUD = %s\n")
		, *LocalRoleString, *RemoteRoleString, *GameModeString, *GameStateString, *PlayerStateString, *PawnName, *PlayerControllerString, *HUDString);

	if (HasAuthority())
	{
		R_Health += 1.0f;
		RU_Mana++;
		FString str = FString::Printf(TEXT("[%s] to Add"), *LocalRoleString);
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, *str);
	}

	FString Value2 = FString::Printf(TEXT("\n\nHealth : %.2f / Mana = %d"), R_Health, RU_Mana);
	Values.Append(Value2);


	DrawDebugString(GetWorld(), GetActorLocation(), Values, nullptr, FColor::White, 0.0f, true);
}

// 서버가 클라이언트의 RU_Mana를 업데이트 할때마다 호출
void ANetworkTestCharacter::OnRep_Mana()
{
	const FString String = FString::Printf(TEXT("Changed current mana %d"), RU_Mana);
	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Blue, *String);
}


void ANetworkTestCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 추가 복제 조건없이 다음 매크로를 사용해 리플리케이션
	DOREPLIFETIME(ANetworkTestCharacter, R_Health);

	// 복제 조건을 명시하여 리플리케이션 (COND_OwnerOnly은 이 액터의 Owner에게만 리플리케이션 하라는 플래그)
	DOREPLIFETIME_CONDITION(ANetworkTestCharacter, RU_Mana, ELifetimeCondition::COND_OwnerOnly);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ANetworkTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 나중에 추가될 입력 처리

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(CommandAction, ETriggerEvent::Triggered, this, &ANetworkTestCharacter::RPCCommand);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void ANetworkTestCharacter::RPCCommand(const FInputActionValue& InputActionValue)
{
	FVector Command = InputActionValue.Get<FVector>();
	if (Command.X == 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,TEXT("Command 1"));
		ServerMsgTest(10);
		ClientMsgTest(11);
		ClientAllMsgTest(12);
	}
	else if (Command.X == 2)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,TEXT("Command 2"));
		ServerMsgTest(20);
		ClientMsgTest(21);
		ClientAllMsgTest(22);
	}
	else if (Command.X == 3)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,TEXT("Command 3"));
		ServerMsgTest(30);
		ClientMsgTest(31);
		ClientAllMsgTest(32);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,TEXT("Error"));
	}
}

void ANetworkTestCharacter::PossessedBy(AController* NewController)
{
	JMSLOG_NET_LOG(Log, TEXT("%s"), TEXT("Begin "));
	Super::PossessedBy(NewController);
	JMSLOG_NET_LOG(Log, TEXT("%s"), TEXT("End"));
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
	UGameplayStatics::OpenLevel(this, *Address);
}

void ANetworkTestCharacter::CallClientTravel(const FString& Address)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetGameInstance()->GetFirstLocalPlayerController());
	if (PlayerController)
	{
		PlayerController->ClientTravel(Address, TRAVEL_Absolute);
	}
}

// 전송될 함수
void ANetworkTestCharacter::ServerMsgTest_Implementation(int32 Value)
{
	FString str = FString::Printf(TEXT("Value %d"), Value);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, str);
}

// ServerMsgTest RPC함수의 (값) 유효성 체크
bool ANetworkTestCharacter::ServerMsgTest_Validate(int32 Value)
{
	return true;
}

void ANetworkTestCharacter::ClientMsgTest_Implementation(int32 Value)
{
	FString str = FString::Printf(TEXT("Value %d"), Value);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, str);
}

bool ANetworkTestCharacter::ClientMsgTest_Validate(int32 Value)
{
	if (Value > 20)
	{
		// 유효하지 않은 값을 사용한 경우 바로 클라이언트를 퇴출시킨다.
		return false;
	}
	return true;
}

void ANetworkTestCharacter::ClientAllMsgTest_Implementation(int32 Value)
{
	FString str = FString::Printf(TEXT("Value %d"), Value);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, str);
}

bool ANetworkTestCharacter::ClientAllMsgTest_Validate(int32 Value)
{
	return true;
}
