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
}
