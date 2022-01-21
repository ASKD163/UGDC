// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UGDC_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDAsset;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> EnemyHUDAsset;

	UUserWidget* EnemyHUD;
	
public:
	virtual void BeginPlay() override;

	void SetEnemyHealthBaeVisibility(bool Visible);

	void UpdateEnemyHealthBarPosition(FVector Location);
	
};
