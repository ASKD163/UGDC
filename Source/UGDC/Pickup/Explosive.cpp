// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


AExplosive::AExplosive()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComponent);

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleComp->SetupAttachment(RootComponent);
}

void AExplosive::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Error, TEXT("%s"), *FString(__FUNCTION__));
	
	if (ParticleOverlap)	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleOverlap, GetActorLocation());
	if (SoundOverlap)	UGameplayStatics::SpawnSound2D(GetWorld(), SoundOverlap);
	Destroy();
}

void AExplosive::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
