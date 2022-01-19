// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pickup/Item.h"
#include "Pickup/Weapon.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 500.f;
	SpringArmComp->SetRelativeRotation(FRotator(-30, 0, 0));
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Attack"));
	AttackSphere->SetupAttachment(RootComponent);
	AttackSphere->SetSphereRadius(300.f);
	AttackSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	GetCharacterMovement()->bOrientRotationToMovement = true; // 向运动方向转身
	GetCharacterMovement()->RotationRate = FRotator(0, 450, 0); //转身速度
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = .2f;
	//JumpMaxHoldTime = 2.f;

	BaseTurnRate = 65;
	LookupRate = 65;

	//不跟随鼠标转身
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	SprintSpeed = 1000.f;
	DefaultSpeed = GetCharacterMovement()->MaxWalkSpeed;
	CurrentState = EState::ES_Normal;
	bIsExhausted = false;
	StaminaDrainRate = 20;

	MaxHealth = 100;
	MaxStamina = 100;

	Health = MaxHealth;
	Stamina = MaxStamina;

	Coins = 0;

	bAttacking = false;
	bClicking = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &ASCharacter::OnAttackSphereBeginOverlap);
	AttackSphere->OnComponentEndOverlap.AddDynamic(this, &ASCharacter::OnAttackSphereEndOverlap);
}

void ASCharacter::OnInteract()
{
	if (ActiveOverlapItem && !bAttacking)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlapItem);
		if (Weapon)	Weapon->Equip(this);
		ActiveOverlapItem = nullptr;
	}
}

void ASCharacter::SetWeapon(AWeapon* Weapon)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = Weapon;
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float Delta = StaminaDrainRate * DeltaTime;
	
	switch (CurrentState)
	{
	case EState::ES_Normal:
		{
			if (bIsExhausted)
			{
				float tmp = Stamina + Delta;
				Stamina = FMath::Clamp(tmp, 0.f, MaxStamina);
				if (Stamina == MaxStamina)
				{
					bIsExhausted = false;
				}
			}
			else
			{
				float tmp = Stamina + Delta;
				Stamina = FMath::Clamp(tmp, 0.f, MaxStamina);
			}
			break;
		}
	case EState::ES_Sprint:
		{
			if (Stamina > 0)
			{
				//if (GetCharacterMovement()->Speed)
				float tmp = Stamina - Delta;
				Stamina = FMath::Clamp(tmp, 0.f, MaxStamina);
			}
			else
			{
				bIsExhausted = true;
				SetState(EState::ES_Normal);
			}
			break;
		}
	default:
		break;		
	}

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRight", this, &ASCharacter::TurnRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::LookUp);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::StopSprint);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASCharacter::OnInteract);
	PlayerInputComponent->BindAction("Click", IE_Pressed, this, &ASCharacter::OnClickBegin);
	PlayerInputComponent->BindAction("Click", IE_Released, this, &ASCharacter::OnClickEnd);
	
}

void ASCharacter::MoveForward(float Value)
{
	if (Controller && !bAttacking)
	{
		const FRotator Rotation = FRotator(0, Controller->GetControlRotation().Yaw , 0);
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASCharacter::MoveRight(float Value)
{
	if (Controller && !bAttacking)
	{
		const FRotator Rotation = FRotator(0, Controller->GetControlRotation().Yaw , 0);
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ASCharacter::TurnRight(float Value)
{
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value * LookupRate * GetWorld()->GetDeltaSeconds());
}

void ASCharacter::SufferDamage(float Damage)
{
	if (Health > 0)
	{
		const float Temp = Health - Damage;
		Health = FMath::Clamp(Temp, 0.f, MaxHealth);
		if (Health <= 0) Die();
	}
}

void ASCharacter::Die()
{
	UE_LOG(LogTemp, Error, TEXT("You Die!"));
}

void ASCharacter::Pickup(EPickupType Type, uint32 Cnt)
{
	switch (Type)
	{
	case Pt_Coin:
		Coins += Cnt;
		break;
	default:
		break;
	}
}

void ASCharacter::StartSprint()
{
	if (!bIsExhausted)	SetState(EState::ES_Sprint);
}

void ASCharacter::StopSprint()
{
	SetState(EState::ES_Normal);
}

void ASCharacter::SetState(EState State)
{
	CurrentState = State;

	switch (CurrentState)
	{
	case EState::ES_Normal:
		GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
		break;
	case EState::ES_Sprint:
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		break;
	default:
		break;		
	}
}

void ASCharacter::OnClickBegin()
{
	bClicking = true;
	if (EquippedWeapon && !bAttacking)
	{
		AttackBegin();
	}
}

void ASCharacter::OnClickEnd()
{
	bClicking = false;
}

void ASCharacter::AttackBegin()
{
	if (TargetEnemy)
	{
		const FRotator CurrentRotation = GetActorRotation();
		
		FRotator TargetRotation =  UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetEnemy->GetActorLocation());
		TargetRotation = FRotator(CurrentRotation.Pitch, TargetRotation.Yaw, CurrentRotation.Roll);
		SetActorRotation(TargetRotation);
	}
	if (!bAttacking)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && AnimMontage && !AnimInstance->Montage_IsPlaying(AnimMontage))
		{
			bAttacking = true;
#if 0
			AnimInstance->Montage_Play(AnimMontage);
			AnimInstance->Montage_JumpToSection(FName("Attack1"), AnimMontage);
#else
			 int Section = FMath::RandRange(0, 1);
			 switch (Section)
			 {
			 case 0:
			 	AnimInstance->Montage_Play(AnimMontage);
			 	AnimInstance->Montage_JumpToSection(FName("Attack1"), AnimMontage);
			 	break;
			 case 1:
			 	AnimInstance->Montage_Play(AnimMontage);
			 	AnimInstance->Montage_JumpToSection(FName("Attack2"), AnimMontage);
			 	break;
			 default:
			 	break;		
			}
#endif
		}
	}
}

void ASCharacter::AttackEnd()
{
	bAttacking = false;
	if (bClicking && EquippedWeapon)
	{
		AttackBegin();
	}

}

void ASCharacter::OnAttackSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!TargetEnemy)
	{
		if (OtherActor)
		{
			AEnemy* Tmp = Cast<AEnemy>(OtherActor);
			if (Tmp) TargetEnemy = Tmp;
		}
	}

}

void ASCharacter::OnAttackSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AEnemy* Tmp = Cast<AEnemy>(OtherActor);
		if (Tmp == TargetEnemy) TargetEnemy = nullptr;
	}
}



