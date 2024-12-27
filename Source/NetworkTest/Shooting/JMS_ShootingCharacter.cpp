// Fill out your copyright notice in the Description page of Project Settings.


#include "JMS_ShootingCharacter.h"

#include "EnhancedInputComponent.h"
#include "Net/UnrealNetwork.h"


AJMS_ShootingCharacter::AJMS_ShootingCharacter()
{
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -89));

	GConfig->GetFloat(TEXT("Shooting"),TEXT("MaxHP"),MaxHP,GGameIni);
	CurrentHP = MaxHP;
}

void AJMS_ShootingCharacter::BeginPlay()
{
	Super::BeginPlay();
}


void AJMS_ShootingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Shoot,ETriggerEvent::Triggered,this,&ThisClass::Shoot);
	}
}

void AJMS_ShootingCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AJMS_ShootingCharacter,CurrentHP);
}

float AJMS_ShootingCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = CurrentHP - DamageAmount;
	SetCurrentHealth(DamageApplied);
	return DamageApplied;
}

void AJMS_ShootingCharacter::Shoot(const FInputActionInstance& InputActionInstance)
{
}

void AJMS_ShootingCharacter::OnRep_CurrentHP()
{
	OnHealthUpdate();
}

void AJMS_ShootingCharacter::OnHealthUpdate()
{
	// 서버든 클라든 나 자신에게서 호출된 함수임을 보장
	if (IsLocallyControlled())
	{
		FString HPMessage = FString::Printf(TEXT("Health [%f / %f]"),CurrentHP,MaxHP);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,HPMessage);
		if (CurrentHP <= 0)
		{
			FString DeathMessage = FString::Printf(TEXT("YOU DIED"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,DeathMessage);
		}
	}

	if (HasAuthority())
	{
		FString HPMessage =  FString::Printf(TEXT("%s now Remaining Health %f"),*GetFName().ToString(),CurrentHP);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,HPMessage);

		
	}
}

void AJMS_ShootingCharacter::SetCurrentHealth(float Hp)
{
	if (HasAuthority())
	{
		CurrentHP = FMath::Clamp(Hp,0.0f,MaxHP);

		// 서버 본인도 호출하기위한 함수 호출
		OnHealthUpdate();
	}
}
