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

	UPROPERTY()
	float AttackInterval;
	
	UPROPERTY()
	bool bAttacking;

	UPROPERTY()
	bool bInColdTime;

	UPROPERTY()
	FTimerHandle AttackCoolDownTimer;
	
public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	class UBoxComponent* WeaponBox;

	UPROPERTY(EditDefaultsOnly, Category = "Attack | Montage")
	class UAnimMontage* AnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Attack | Effect")
	UParticleSystem* InteractParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Attack | Sound")
	USoundBase* ReactSound;
	
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

	UFUNCTION(BlueprintCallable)
	void OnWeaponBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);

	UFUNCTION()
	void OnWeaponBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void AttackBegin();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void BeginOverlap();

	UFUNCTION(BlueprintCallable)
	void EndOverlap();

	UFUNCTION()
	void OnAttackTimeout();

	UFUNCTION()
	void UpdateState();
};
