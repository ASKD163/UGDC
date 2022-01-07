// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class UGDC_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Component")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Float")
	FVector StartPoint;

	UPROPERTY(EditInstanceOnly, meta = (MakeEditWidget = "true"), Category = "Float")
	FVector EndPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Float")
	float InterpSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Float")
	bool bInterping;
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ToggleInterp();

};
