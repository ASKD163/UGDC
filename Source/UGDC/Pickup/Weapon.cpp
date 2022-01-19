// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "UGDC/SCharacter.h"
#include "UGDC/Enemy/Enemy.h"

AWeapon::AWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	Mesh->SetupAttachment(MeshComp);

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(Mesh);
	WeaponBox->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	bRotate = true;
	RotateSpeed = FRotator(0,180,0);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponBoxBeginOverlap);
	WeaponBox->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnWeaponBoxEndOverlap);
}

void AWeapon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor)
	{
		ASCharacter* Character = Cast<ASCharacter>(OtherActor);
		if (Character)
		{
			if (!Character->ActiveOverlapItem) Character->ActiveOverlapItem = this;

			TArray<FStringFormatArg> FormatArrays;
			FormatArrays.Add(GetName());
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Blue,
			                                              FString::Format(TEXT("Press E To Equip {0}"), FormatArrays),
			                                              true, FVector2D(4, 4));
			
		}
	}
}

void AWeapon::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor)
	{
		ASCharacter* Character = Cast<ASCharacter>(OtherActor);
		if (Character)
		{
			if (Character->ActiveOverlapItem == this) Character->ActiveOverlapItem = nullptr;
			
			if (GEngine) GEngine->ClearOnScreenDebugMessages();
		}
	}
	
}

void AWeapon::Equip(ASCharacter* Character)
{
	if (Character)
	{
		Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		Mesh->SetSimulatePhysics(false);
		
		AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
		bRotate = false;
		ParticleComp->SetActive(false);

		SphereComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AItem::OnBeginOverlap);
		SphereComponent->OnComponentEndOverlap.RemoveDynamic(this, &AItem::OnEndOverlap);
		if (GEngine) GEngine->ClearOnScreenDebugMessages();
		if (SoundEquipped) UGameplayStatics::PlaySound2D(this, SoundEquipped);
		Character->SetWeapon(this);
	}
}

void AWeapon::OnWeaponBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy)
		{
			const FVector Location = SweepResult.Location;
			//DrawDebugSphere(GetWorld(), Location, 10, 12, FColor::Red, true, 3.f);
			if (Enemy->InteractParticle)
				UGameplayStatics::SpawnEmitterAtLocation(this, Enemy->InteractParticle, Mesh->GetComponentLocation());
	
			if (Enemy->ReactSound)
				UGameplayStatics::PlaySound2D(this, Enemy->ReactSound);
		}
	}
}

void AWeapon::OnWeaponBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AWeapon::BeginOverlap()
{
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::EndOverlap()
{
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
