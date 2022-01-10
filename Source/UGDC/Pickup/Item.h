// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class UGDC_API AItem : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Item | Collision")
	class USphereComponent* SphereComponent;
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
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
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                            const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
