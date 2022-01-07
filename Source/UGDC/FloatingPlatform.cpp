// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	InterpSpeed = 4;
	bInterping = true;

}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	StartPoint = GetActorLocation();
	EndPoint += StartPoint;
	
	FTimerHandle ToggleHandle;
	GetWorldTimerManager().SetTimer(ToggleHandle, this, &AFloatingPlatform::ToggleInterp, 5.f, true);
	
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector Current = GetActorLocation();

	FVector TargetLocation;

	if (bInterping)
	{
		TargetLocation = FMath::VInterpTo(Current, EndPoint, DeltaTime, InterpSpeed);
	}

	else
	{
		TargetLocation = FMath::VInterpTo(Current, StartPoint, DeltaTime, InterpSpeed);
	}

	SetActorLocation(TargetLocation);

}

void AFloatingPlatform::ToggleInterp()
{
	bInterping = !bInterping;
}

