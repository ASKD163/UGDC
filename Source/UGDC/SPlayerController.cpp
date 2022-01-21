// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"

#include "Blueprint/UserWidget.h"

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDAsset)
	{
		UUserWidget* HUD = CreateWidget<UUserWidget>(this, HUDAsset);

		HUD->AddToViewport();
	}

	if (EnemyHUDAsset)
	{
		EnemyHUD = CreateWidget<UUserWidget>(this, EnemyHUDAsset);
		EnemyHUD->AddToViewport();
		EnemyHUD->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ASPlayerController::SetEnemyHealthBaeVisibility(bool Visible)
{
	if (EnemyHUD)
	{
		if (Visible)
		{
			EnemyHUD->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			EnemyHUD->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ASPlayerController::UpdateEnemyHealthBarPosition(FVector Location)
{
	if (EnemyHUD)
	{
		const FVector TargetLocation = FVector(Location.X , Location.Y, Location.Z + 150);
		FVector2D ScreenLocation;
		ProjectWorldLocationToScreen(TargetLocation, ScreenLocation);
		const FVector2D Size = FVector2D(150, 20);

		ScreenLocation.X -= 0.25 * Size.X;
		EnemyHUD->SetPositionInViewport(ScreenLocation);
		EnemyHUD->SetDesiredSizeInViewport(Size);
	}
}

