// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class UGDC_API AWeapon : public AItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	USkeletalMeshComponent* Mesh;

public:
	AWeapon();
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                            const FHitResult& SweepResult) override;
	
	void Equip(ACharacter* Character);
	
};
