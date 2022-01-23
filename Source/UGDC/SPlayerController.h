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

	UPROPERTY()
	UUserWidget* EnemyHUD;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseHUDAsset;

	UPROPERTY()
	UUserWidget* PauseHUD;

	UPROPERTY()
	bool bPause;
	
public:
	virtual void BeginPlay() override;

	void SetEnemyHealthBarVisibility(bool Visible);

	void UpdateEnemyHealthBarPosition(FVector Location);

	void SetPauseUIVisibility(bool Visibility);

	UFUNCTION(BlueprintCallable)
	void TogglePause();
};
