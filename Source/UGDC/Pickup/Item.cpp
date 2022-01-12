// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = SphereComponent;
		
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComponent);

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleComp->SetupAttachment(RootComponent);

	bRotate = false;
	RotateSpeed = FRotator::ZeroRotator;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnBeginOverlap);

	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AItem::OnEndOverlap);
	
}

void AItem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bRotate)
	{
		FRotator CurrentRotation = MeshComp->GetComponentRotation();
		CurrentRotation += (RotateSpeed * DeltaSeconds);

		MeshComp->SetWorldRotation(CurrentRotation);
		
	}
}

void AItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__func__));
	
	if (ParticleOverlap)	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleOverlap, GetActorLocation());
	if (SoundOverlap)	UGameplayStatics::SpawnSound2D(GetWorld(), SoundOverlap);
	//Destroy();
}

void AItem::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
}
