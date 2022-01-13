// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "UGDC/SCharacter.h"

AWeapon::AWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	Mesh->SetupAttachment(MeshComp);

	bRotate = true;
	RotateSpeed = FRotator(0,180,0);
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
