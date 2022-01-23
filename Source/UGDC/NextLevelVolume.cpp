// Fill out your copyright notice in the Description page of Project Settings.


#include "NextLevelVolume.h"

#include "SCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANextLevelVolume::ANextLevelVolume()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Component"));
	RootComponent = BoxComponent;

	LevelName = "Level1";
}

// Called when the game starts or when spawned
void ANextLevelVolume::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ANextLevelVolume::OnBoxComponentBeginOverlap);
	
}

void ANextLevelVolume::OnBoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ASCharacter* SCharacter = Cast<ASCharacter>(OtherActor);
		if (SCharacter)
		{
			const FName CurrentLevelName(GetWorld()->GetMapName());
			if (CurrentLevelName != LevelName)
				UGameplayStatics::OpenLevel(GetWorld(), LevelName);
		}
	}
}

