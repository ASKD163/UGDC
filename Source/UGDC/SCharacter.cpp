// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Pickup/Item.h"

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

	MaxHealth = 100;
	MaxStamina = 100;

	Health = MaxHealth;
	Stamina = MaxStamina;

	Coins = 0;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	
}

void ASCharacter::MoveForward(float Value)
{
	if (Controller)
	{
		const FRotator Rotation = FRotator(0, Controller->GetControlRotation().Yaw , 0);
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASCharacter::MoveRight(float Value)
{
	if (Controller)
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
	// UCharacterMovementComponent* CMC = GetCharacterMovement();
	// if (CMC)
	// {
	// 	DefaultSpeed = CMC->GetMaxSpeed();
	// 	
	// }
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	
}

void ASCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
}



