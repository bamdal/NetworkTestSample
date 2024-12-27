// Fill out your copyright notice in the Description page of Project Settings.


#include "JMS_ItemBuffComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "NetworkTest/CoinFramework/JMS_CoinCharacter.h"


// Sets default values for this component's properties
UJMS_ItemBuffComponent::UJMS_ItemBuffComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



// Called when the game starts
void UJMS_ItemBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UJMS_ItemBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
// 서버가 호출한 멀티캐스트를 클라가 실행 ( 데디케이티드 서버는 로컬 플레이어가 없기 때문에 이 함수는 NM_Client만 호출됨 )
void UJMS_ItemBuffComponent::MulticastSpeedBuff_Implementation(float BaseSpeed)
{
	if(CoinGameCharacter == nullptr || CoinGameCharacter->GetCharacterMovement() == nullptr)
	{
		return;
	}
	CoinGameCharacter->GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%f"), CoinGameCharacter->GetCharacterMovement()->MaxWalkSpeed));
}

void UJMS_ItemBuffComponent::SpeedBuffServerProc(float BuffSpeed, float BuffTime)
{
	if(CoinGameCharacter == nullptr || CoinGameCharacter->GetCharacterMovement() == nullptr)
	{
		return;
	}
	
	// 서버에서 먼저 movement 수정(데디케이티드 서버시 필요함)
	CoinGameCharacter->GetCharacterMovement()->MaxWalkSpeed = BuffSpeed;
	MulticastSpeedBuff(BuffSpeed);
	
	CoinGameCharacter->GetWorldTimerManager().SetTimer(SpeedBuffTimerHandle,this,&UJMS_ItemBuffComponent::ResetSpeedBuff,BuffTime);
	
}
          
void UJMS_ItemBuffComponent::ResetSpeedBuff()
{
	if(CoinGameCharacter == nullptr || CoinGameCharacter->GetCharacterMovement() == nullptr)
	{
		return;
	}
	
	CoinGameCharacter->GetCharacterMovement()->MaxWalkSpeed = InitialBaseSpeed;
	MulticastSpeedBuff(InitialBaseSpeed);
}

