// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UGDC/SCharacter.h"

// Sets default values
AEnemy::AEnemy()
{
	DetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectSphere"));
	DetectSphere->SetupAttachment(RootComponent);
	DetectSphere->SetSphereRadius(800);

	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	AttackSphere->SetupAttachment(RootComponent);
	AttackSphere->SetSphereRadius(300);

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(GetMesh(), "Front_Right_Knee");
	WeaponBox->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	MoveState = EMoveState::MS_Idle;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	DetectSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnDetectSphereBeginOverlap);
	DetectSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnDetectSphereEndOverlap);

	AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnAttackSphereBeginOverlap);
	AttackSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnAttackSphereEndOverlap);

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnWeaponBoxBeginOverlap);
	WeaponBox->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnWeaponBoxEndOverlap);

	AIController = Cast<AAIController>(GetController());

	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::OnDetectSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!TargetMen)
	{
		if (OtherActor)
		{
			ASCharacter* Character = Cast<ASCharacter>(OtherActor);
			if (Character)
			{
				TargetMen = Character;
				if (MoveState != EMoveState::MS_Attack)
					MoveState = EMoveState::MS_MoveToTarget;
				MoveToTarget();
			}
		}
	}
}

void AEnemy::OnAttackSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HittingMen)
	{
		if (OtherActor)
		{
			ASCharacter* Character = Cast<ASCharacter>(OtherActor);
			if (Character)
			{
				HittingMen = Character;
				AttackBegin();
				//MoveState = EMoveState::MS_Attack;
				if (AIController) AIController->StopMovement();
			}
		}
	}

}

void AEnemy::OnDetectSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ASCharacter* Character = Cast<ASCharacter>(OtherActor);
		if (Character == TargetMen)
		{
			TargetMen = nullptr;
			if (MoveState != EMoveState::MS_Attack) MoveState = EMoveState::MS_Idle;
		}
		if (AIController) AIController->StopMovement();
	}
}

void AEnemy::OnAttackSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ASCharacter* Character = Cast<ASCharacter>(OtherActor);
		if (Character == HittingMen)
		{
			//TargetMen = HittingMen;
			HittingMen = nullptr;
		}
	}
}

void AEnemy::MoveToTarget()
{
	if (TargetMen)
	{
		if (AIController)
		{
			FAIMoveRequest MoveRequest;

			//移动的目标Actor， 使用 SetGoalLocation() 可设置移动的目标位置
			MoveRequest.SetGoalActor(TargetMen);
			//离目标位置或Actor多远判定为移动完成
			MoveRequest.SetAcceptanceRadius(10.f);
			
			FNavPathSharedPtr OutPath;
				
			AIController->MoveTo(MoveRequest, &OutPath);
		}
	}
	else
	{
		MoveState = EMoveState::MS_Idle;
	}
}

void AEnemy::OnWeaponBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ASCharacter* SCharacter = Cast<ASCharacter>(OtherActor);
		if (SCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit"));
			
			const FVector Location = SweepResult.Location;
			DrawDebugSphere(GetWorld(), Location, 10, 12, FColor::Red, true, 3.f);
			if (SCharacter->InteractParticle)
				UGameplayStatics::SpawnEmitterAtLocation(this, SCharacter->InteractParticle, WeaponBox->GetComponentLocation());
	
			if (SCharacter->ReactSound)
				UGameplayStatics::PlaySound2D(this, SCharacter->ReactSound);
		}
	}
}

void AEnemy::OnWeaponBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AEnemy::AttackBegin()
{
	MoveState = EMoveState::MS_Attack;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && AnimMontage && !AnimInstance->Montage_IsPlaying(AnimMontage))
	{
		AnimInstance->Montage_Play(AnimMontage);
		AnimInstance->Montage_JumpToSection("Attack", AnimMontage);
	}

}

void AEnemy::AttackEnd()
{
	if (HittingMen)
	{
		MoveToTarget();
		AttackBegin();
	}
	else if (TargetMen)
	{
		MoveState = EMoveState::MS_MoveToTarget;
		MoveToTarget();
	}
	else
	{
		MoveState = EMoveState::MS_Idle;
	}
}

void AEnemy::BeginOverlap()
{
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::EndOverlap()
{
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
