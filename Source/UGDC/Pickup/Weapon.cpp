// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Components/SphereComponent.h"
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
		if (Character) Equip(Character);
	}
}

void AWeapon::Equip(ACharacter* Character)
{
	if (Character)
	{
		Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		Mesh->SetSimulatePhysics(false);
		
		AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
		bRotate = false;
		ParticleComp->SetActive(false);

		SphereComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AItem::OnBeginOverlap);
	}
}
