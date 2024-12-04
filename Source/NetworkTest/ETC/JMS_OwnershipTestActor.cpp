// Fill out your copyright notice in the Description page of Project Settings.


#include "JMS_OwnershipTestActor.h"
#include "JMSDebugMecros.h"
#include "Kismet/GameplayStatics.h"
#include "NetworkTest/Charater/NetworkTestCharacter.h"

// Sets default values
AJMS_OwnershipTestActor::AJMS_OwnershipTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// true로 하면 복제할 엑터임을 알림
	this->bReplicates = true;
	
}

// Called when the game starts or when spawned
void AJMS_OwnershipTestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJMS_OwnershipTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), OwnershipRadius, 12, FColor::Red, true);

	// 내 Role이 ROLE_Authority이라면 = 내가 서버라면
	if(HasAuthority())
	{
		AActor* NextOwner = nullptr;
		float MinDistance = OwnershipRadius;
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(this,ANetworkTestCharacter::StaticClass(),Actors);
		for(AActor* Actor : Actors)
		{
			const float Distance = GetDistanceTo(Actor);
			if(Distance <= MinDistance)
			{
				MinDistance = Distance;
				NextOwner = Actor;
			}
		}
		
		if(GetOwner() != NextOwner)
		{
			SetOwner(NextOwner);
		}
	}


	const FString OwnerString = GetOwner() != nullptr ? GetOwner()->GetName() : TEXT("No Owner");
	const FString ConnnectionString = GetNetConnection() != nullptr ?TEXT("Valid Connection") : TEXT("InValid Connection");
	
	const FString Values = FString::Printf(TEXT("Owner: %s\nConnection : %s"), *OwnerString, *ConnnectionString);
	DrawDebugString(GetWorld(),GetActorLocation(),Values,nullptr,FColor::White,0.0f,true);

}

