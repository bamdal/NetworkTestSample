// Fill out your copyright notice in the Description page of Project Settings.


#include "JMS_ShootingCharacter.h"

#include "EnhancedInputComponent.h"
#include "JMS_Projectile.h"
#include "Net/UnrealNetwork.h"
#include "ProceduralMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProceduralMeshComponent/Public/KismetProceduralMeshLibrary.h"
#include "Rendering/SkeletalMeshRenderData.h"


AJMS_ShootingCharacter::AJMS_ShootingCharacter()
{
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -89));

	GConfig->GetFloat(TEXT("Shooting"),TEXT("MaxHP"),MaxHP,GGameIni);
	CurrentHP = MaxHP;

	ProjectileClass = AJMS_Projectile::StaticClass();

	FireRate = 0.25f;
	bIsFiringWeapon = false;
	CutBoneName = TEXT("spine_05");

	bReplicates = true;
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
		EnhancedInputComponent->BindAction(IA_Shoot,ETriggerEvent::Ongoing,this,&ThisClass::Shoot);
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
	StartFire();
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
				// 서버에 요청
			ServerHandleCutting();

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

void AJMS_ShootingCharacter::StartFire()
{
	if (!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		GetWorld()->GetTimerManager().SetTimer(FiringTimer,this,&ThisClass::StopFire,FireRate,false);
		HandleFire();
	}
	
}

void AJMS_ShootingCharacter::StopFire()
{
	bIsFiringWeapon = false;
}



void AJMS_ShootingCharacter::HandleFire_Implementation()
{

	//FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * 100.0f) + (GetActorUpVector() * 50.0f);
	FVector SpawnLocation = GetActorLocation() + (GetActorRotation().Vector() * 100.f) + (GetActorUpVector() * 50.0f);
	FRotator SpawnRotation = GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.Owner = this;
	GetWorld()->SpawnActor<AJMS_Projectile>(SpawnLocation,SpawnRotation,SpawnParams);
}

UProceduralMeshComponent* AJMS_ShootingCharacter::CreateProceduralMeshFromBone(USkeletalMeshComponent* SkeletalMesh, const FName& BoneName)
{
   if (!SkeletalMesh || !SkeletalMesh->GetSkeletalMeshAsset())
    {
        return nullptr;
    }

    USkeletalMesh* SkeletalMeshAsset = SkeletalMesh->GetSkeletalMeshAsset();
    const int32 BoneIndex = SkeletalMesh->GetBoneIndex(BoneName);

    if (BoneIndex == INDEX_NONE)
    {
        UE_LOG(LogTemp, Warning, TEXT("Bone '%s' not found!"), *BoneName.ToString());
        return nullptr;
    }

    // 버텍스 및 트라이앵글 데이터를 수집하기 위한 변수
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FLinearColor> VertexColors;
    TArray<FProcMeshTangent> Tangents;

    // 본에 연결된 버텍스 수집
    const FSkeletalMeshRenderData* RenderData = SkeletalMeshAsset->GetResourceForRendering();
    const FSkeletalMeshLODRenderData& LODData = RenderData->LODRenderData[0];
    const FSkelMeshRenderSection& Section = LODData.RenderSections[0];

    for (uint32 VertexIndex = 0; VertexIndex < Section.NumVertices; ++VertexIndex)
    {
        const int32 MappedBoneIndex = LODData.SkinWeightVertexBuffer.GetBoneIndex(VertexIndex, 0);

        if (MappedBoneIndex == BoneIndex)
        {
            FVector VertexPosition = static_cast<FVector>(LODData.StaticVertexBuffers.PositionVertexBuffer.VertexPosition(VertexIndex));
            Vertices.Add(VertexPosition);

            FVector Normal = static_cast<FVector>(LODData.StaticVertexBuffers.StaticMeshVertexBuffer.VertexTangentZ(VertexIndex));
            Normals.Add(Normal);

            FVector2D UV = static_cast<FVector2D>(LODData.StaticVertexBuffers.StaticMeshVertexBuffer.GetVertexUV(VertexIndex, 0));
            UVs.Add(UV);

            VertexColors.Add(FLinearColor::White);
            Tangents.Add(FProcMeshTangent());
        }
    }

    // 삼각형 인덱스 데이터 생성 (단순 연결)
    for (int32 Index = 0; Index < Vertices.Num() - 2; Index++)
    {
        Triangles.Add(Index);
        Triangles.Add(Index + 1);
        Triangles.Add(Index + 2);
    }

    // 프로시저럴 메시 생성
    UProceduralMeshComponent* ProcMesh = NewObject<UProceduralMeshComponent>(this);
    ProcMesh->RegisterComponent();
    ProcMesh->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::KeepWorldTransform);

    ProcMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);

    return ProcMesh;
}

void AJMS_ShootingCharacter::AttachMeshToBone(UProceduralMeshComponent* MeshPart, const FName& BoneName)
{

	MeshPart->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, BoneName);
}

void AJMS_ShootingCharacter::ServerHandleCutting_Implementation()
{
	StartCutting();
	if (HasAuthority())
	{
		StartCutting_Implementation();
	}
}

void AJMS_ShootingCharacter::StartCutting_Implementation()
{
	ApplySliceEffect(CutBoneName);
}
void AJMS_ShootingCharacter::ApplySliceEffect(const FName& BoneName)
{

	if (IsLocallyControlled())
	{
		GetInstigatorController()->UnPossess();
	}
	if (GetMesh())
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll")); // 필요에 따라 충돌 프로필 설정
	}
	
	// 1. 본 트랜스폼 가져오기
	FTransform BoneTransform = GetMesh()->GetBoneTransform(BoneName);

	// 2. 본에 해당하는 프로시저럴 메시 생성
	UProceduralMeshComponent* ProceduralMesh = CreateProceduralMeshFromBone(GetMesh(), BoneName);
	if (!ProceduralMesh) return;

	// 3. SliceProceduralMesh로 절단
	FVector PlanePosition = BoneTransform.GetLocation();
	FVector PlaneNormal = FVector::UpVector;

	UProceduralMeshComponent* OtherHalfMesh = nullptr; // 절단 후 다른 절반을 받을 변수
	UKismetProceduralMeshLibrary::SliceProceduralMesh(
		ProceduralMesh,
		PlanePosition,
		PlaneNormal,
		true,
		OtherHalfMesh,
		EProcMeshSliceCapOption::CreateNewSectionForCap,
		nullptr
	);

	// 4. 절단된 메시를 본에 어태치
	AttachMeshToBone(ProceduralMesh, BoneName);
	if (OtherHalfMesh)
	{
		FName ParentBoneName = GetMesh()->GetParentBone(BoneName);
		AttachMeshToBone(OtherHalfMesh, ParentBoneName);
	}

	// 5. 본의 물리 제약 해제
	ProceduralMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 쿼리 및 물리 활성화
	
	GetMesh()->BreakConstraint(FVector::ZeroVector, FVector::UpVector, BoneName);

	ProceduralMesh->SetSimulatePhysics(true);
	
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 쿼리 및 물리 활성화
	
	GetMesh()->SetSimulatePhysics(true);
	OtherHalfMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->WakeAllRigidBodies();      // 모든 물리 바디 깨우기

	GetMesh()->bBlendPhysics = true;

	GetMesh()->bReplicatePhysicsToAutonomousProxy = true;


	
	// 6. 임펄스 적용 (옵션)
	if (ProceduralMesh)
	{
		ProceduralMesh->AddImpulse(FVector::UpVector * 3000000.0f, BoneName,true);
	}
	

	
}
