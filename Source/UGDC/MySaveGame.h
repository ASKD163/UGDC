// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

USTRUCT()
struct FPlayerSlot
{
	GENERATED_BODY()
	
	UPROPERTY()
	float MaxHealth;

	UPROPERTY()
	float Stamina;

	UPROPERTY()
	int Coins;
	
};
/**
 * 
 */
UCLASS()
class UGDC_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	FPlayerSlot PlayerSlot;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;
	
};
