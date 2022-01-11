// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "UGDC/SCharacter.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;

	bRotate = true;
	RotateSpeed = FRotator(0,180,0);
}

void APickup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Error, TEXT("%s"), *FString(__FUNCTION__));
}

void APickup::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor)
	{
		ASCharacter* Player = Cast<ASCharacter>(OtherActor);
		if (Player) Player->Pickup(Type, 1);
	}
}

