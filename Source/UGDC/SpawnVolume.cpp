// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SCharacter.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
	BoxExtent = FVector(100, 100, 50);
		
	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	RootComponent = SpawningBox;
	SpawningBox->SetBoxExtent(BoxExtent);

}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ASpawnVolume::GetSpawnPoint()
{
	const FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), BoxExtent);
	return Point;
}

FVector ASpawnVolume::GetSpawnPoint_Dummy()
{
	return GetSpawnPoint();
}

void ASpawnVolume::SpawnMyPawn_Implementation(UClass* PawnClass, FVector const& Location)
{
	if (PawnClass)
	{
		UWorld* World = GetWorld();
		if (World)
			ASCharacter* SCharacter = World->SpawnActor<ASCharacter>(PawnClass, Location, FRotator::ZeroRotator);
	}
}


