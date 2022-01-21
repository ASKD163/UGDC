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

	UPROPERTY(EditDefaultsOnly, Category = "Item | Sound")
	USoundBase* SoundEquipped;

	UPROPERTY(EditDefaultsOnly, Category = "Item | Damage")
	float Damage;

	UPROPERTY(EditInstanceOnly, Category = "Item | Damage")
	TSubclassOf<UDamageType> DamageType;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBoxComponent* WeaponBox;
	
	AWeapon();

	virtual void BeginPlay() override;
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                            const FHitResult& SweepResult) override;

	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	void Equip(class ASCharacter* Character);

	UFUNCTION(BlueprintCallable)
	void OnWeaponBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								 const FHitResult& SweepResult);

	UFUNCTION()
	void OnWeaponBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void BeginOverlap();

	UFUNCTION(BlueprintCallable)
	void EndOverlap();
};
