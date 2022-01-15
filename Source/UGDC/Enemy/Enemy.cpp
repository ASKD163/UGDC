// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
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

	AIController = Cast<AAIController>(GetController());
	
}

void AEnemy::OnDetectSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!TargetMen)
	{
		if (OtherActor)
		{
			TargetMen = Cast<ASCharacter>(OtherActor);
			if (TargetMen)
			{
				if (MoveState != EMoveState::MS_Attack)	MoveState = EMoveState::MS_MoveToTarget;
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
				MoveState = EMoveState::MS_Attack;
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
			TargetMen = HittingMen;
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
