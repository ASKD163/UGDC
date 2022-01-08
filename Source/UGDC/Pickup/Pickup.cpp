// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComponent);

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleComp->SetupAttachment(RootComponent);

	bRotate = true;
	RotateSpeed = FRotator::ZeroRotator;
}

void APickup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (bRotate)
	{
		FRotator CurrentRotation = MeshComp->GetComponentRotation();
		CurrentRotation += (RotateSpeed * DeltaSeconds);

		MeshComp->SetWorldRotation(CurrentRotation);
		
	}
}

void APickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Error, TEXT("%s"), *FString(__FUNCTION__));

	if (ParticleOverlap)	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleOverlap, GetActorLocation());
	if (SoundOverlap)	UGameplayStatics::SpawnSound2D(GetWorld(), SoundOverlap);
	Destroy();
}

void APickup::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
