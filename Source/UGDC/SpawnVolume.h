// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class UGDC_API ASpawnVolume : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Component")
	class UBoxComponent* SpawningBox;
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditInstanceOnly, Category = "SpawnBox")
	FVector BoxExtent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnBox")
	TSubclassOf<class ASCharacter> PawnToSpawn;
	
	UFUNCTION(BlueprintPure)
	FVector GetSpawnPoint();

	UFUNCTION(BlueprintCallable)
	FVector GetSpawnPoint_Dummy();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SpawnMyPawn(UClass* PawnClass, FVector const& Location);
};
