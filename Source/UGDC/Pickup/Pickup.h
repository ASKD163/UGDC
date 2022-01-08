// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class UGDC_API APickup : public AItem
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Item | Mesh")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Item | Particle")
	UParticleSystem* ParticleOverlap;

	UPROPERTY(VisibleAnywhere, Category = "Item | Particle")
	UParticleSystemComponent* ParticleComp;

	UPROPERTY(EditDefaultsOnly, Category = "Item | Sound")
	USoundBase* SoundOverlap;

	UPROPERTY(EditDefaultsOnly, Category = "Item | Rotate")
	bool bRotate;

	UPROPERTY(EditDefaultsOnly, Category = "Item | Rotate")
	FRotator RotateSpeed;

public:
	APickup();

	virtual void Tick(float DeltaSeconds) override;
	
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
					  int32 OtherBodyIndex) override;
	
};
