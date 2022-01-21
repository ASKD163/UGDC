// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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

	AttackInterval = 3.f;

	MaxHealth = 100;
	Damage = 60;
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

	Health = MaxHealth;
}

void AEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (TargetMen && !bAttacking)
	{
		const FRotator CurrentRotation = GetActorRotation();
		
		FRotator TargetRotation =  UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetMen->GetActorLocation());
		TargetRotation = FRotator(CurrentRotation.Pitch, TargetRotation.Yaw, CurrentRotation.Roll);
		SetActorRotation(TargetRotation);
	}
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
				MoveState = EMoveState::MS_Attack;
				if (!bInColdTime) AttackBegin();
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
		if (Character && Character == TargetMen)
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
		if (Character && Character == HittingMen)
		{
			//TargetMen = HittingMen;
			HittingMen = nullptr;
			//UpdateState();
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
			const FDamageEvent DamageEvent;

			if (DamageType)
				UGameplayStatics::ApplyDamage(SCharacter, Damage, AIController, this, DamageType);
			
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
	if (HittingMen)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && AnimMontage && !AnimInstance->Montage_IsPlaying(AnimMontage))
		{
			bAttacking = true;
			AnimInstance->Montage_Play(AnimMontage);
			AnimInstance->Montage_JumpToSection("Attack", AnimMontage);

			bInColdTime = true;
			GetWorldTimerManager().ClearTimer(AttackCoolDownTimer);
			GetWorldTimerManager().SetTimer(AttackCoolDownTimer, this, &AEnemy::OnAttackTimeout, AttackInterval);
		}
	}
}

void AEnemy::AttackEnd()
{
	bAttacking = false;
	
	UpdateState();
}

void AEnemy::BeginOverlap()
{
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::EndOverlap()
{
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::OnAttackTimeout()
{
	bInColdTime = false;

	UpdateState();
}

void AEnemy::UpdateState()
{
	if (MoveState == EMoveState::MS_Dead) return;
	if (HittingMen)
	{
		MoveToTarget();
		if (!bInColdTime) AttackBegin();
	}
	else if (TargetMen)
	{
		if (!bAttacking)
		{
			MoveState = EMoveState::MS_MoveToTarget;
			MoveToTarget();
		}
	}
	else
	{
		MoveState = EMoveState::MS_Idle;
	}
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AnimMontage)
	{
		AnimInstance->Montage_Play(AnimMontage);
		AnimInstance->Montage_JumpToSection(FName("Death"), AnimMontage);
	}
	
	MoveState = EMoveState::MS_Dead;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetectSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                         AActor* DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("B"));
	if (Health > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("A"));
		Health = FMath::Clamp(Health - DamageAmount, 0.f, MaxHealth);
		if (Health <= 0) Die();
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AEnemy::DeadEnd()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) AnimInstance->Montage_Pause();
}
