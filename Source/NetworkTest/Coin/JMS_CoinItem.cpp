// Fill out your copyright notice in the Description page of Project Settings.


#include "JMS_CoinItem.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "NetworkTest/CoinFramework/JMS_CoinCharacter.h"
#include "NetworkTest/CoinFramework/JMS_CoinGameMode.h"
#include "NetworkTest/CoinFramework/JMS_CoinGameState.h"
#include "NetworkTest/Component/JMS_ItemBuffComponent.h"


// Sets default values
AJMS_CoinItem::AJMS_CoinItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Mesh->SetCollisionProfileName(TEXT("OverlapAll"));

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	RotatingMovementComponent->RotationRate = FRotator(0.0f, 90.0f, 0.0f);

	// 원격에서 리플리케이트 되게 수락
	bReplicates = true;

	
}

// Called when the game starts or when spawned
void AJMS_CoinItem::BeginPlay()
{
	Super::BeginPlay();
	
	if(HasAuthority())
	{
		Mesh->OnComponentBeginOverlap.AddDynamic(this,&AJMS_CoinItem::OnBeginOverlap);
		
	}
}



void AJMS_CoinItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{


	
	AJMS_CoinCharacter* Character = Cast<AJMS_CoinCharacter>(Other);

	
	if(Character == nullptr)
	{

		return;
	}

	
	AJMS_CoinGameState* GameState = Cast<AJMS_CoinGameState>(GetWorld()->GetGameState());
	if(GameState != nullptr)
	{
		GameState->RemovePickup();
		
	}
	AJMS_CoinGameMode* GameMode = Cast<AJMS_CoinGameMode>(GetWorld()->GetAuthGameMode());
	if(GameMode == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0,FColor::Red,TEXT("NullGameMode"));
		return;
	}

	Character->ClientPlaySound2D(PickupSound);
	Character->AddScore(GameMode->PickupPoint);
	Character->AddPickup();
	if(ItemTypeSpeed)
	{
		UJMS_ItemBuffComponent* ItemBuff = Character->GetItemBuff();
		if(ItemBuff != nullptr)
		{
			ItemBuff->SpeedBuffServerProc(BaseSpeedBuff,SpeedBuffTime);
		}
	}
	
	Destroy();

}

