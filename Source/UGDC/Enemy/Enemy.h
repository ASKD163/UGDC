// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


UENUM(BlueprintType)
enum class EMoveState: uint8
{
	MS_Idle				UMETA(DisplayName = "Idle"),
	MS_MoveToTarget		UMETA(DisplayName = "MoveToTarget"),
	MS_Attack			UMETA(DisplayName = "Attack"),
};

UCLASS()
class UGDC_API AEnemy : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* DetectSphere;

	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* AttackSphere;

	UPROPERTY()
	class AAIController* AIController;
	
public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EMoveState MoveState;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ASCharacter* HittingMen;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ASCharacter* TargetMen;
	
	UFUNCTION()
	void OnDetectSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                const FHitResult& SweepResult);

	UFUNCTION()
	void OnAttackSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
									const FHitResult& SweepResult);

	UFUNCTION()
	void OnDetectSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnAttackSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void MoveToTarget();
};
