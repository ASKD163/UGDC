// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "UGDC/SCharacter.h"


AExplosive::AExplosive()
{
	Damage = 30.f;
	
	bRotate = false;
	RotateSpeed = FRotator::ZeroRotator;
}

void AExplosive::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Error, TEXT("%s"), *FString(__FUNCTION__));
}

void AExplosive::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	
	ASCharacter* Player = Cast<ASCharacter>(OtherActor);
	if (Player) Player->SufferDamage(Damage);
}
