// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UENUM()
enum class EState: uint8
{
	ES_Normal UMETA(DisplayName = "Normal"),

	ES_Sprint UMETA(DisplayName = "Sprint"),
};

UCLASS()
class UGDC_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Component")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	class USphereComponent* AttackSphere;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float BaseTurnRate;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float LookupRate;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DefaultSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintSpeed;

	/*per second*/
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	float StaminaDrainRate;

	UPROPERTY()
	class ASPlayerController* SPlayerController;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AEnemy> EnemyFilter;
	
public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "UI")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	float Stamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	float MaxStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	int Coins;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	EState CurrentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsExhausted;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AEnemy* TargetEnemy;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AWeapon* EquippedWeapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AItem* ActiveOverlapItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack | Montage")
	UAnimMontage* AnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Attack | Effect")
	UParticleSystem* InteractParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Attack | Sound")
	USoundBase* ReactSound;

	bool bAttacking;

	bool bClicking;

	UFUNCTION()
	void OnInteract();

	void SetWeapon(AWeapon* Weapon);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnRight(float Value);

	void LookUp(float Value);

	void SufferDamage(float Damage);

	void Die();

	void Pickup(enum EPickupType Type, uint32 Cnt);

	UFUNCTION()
	void StartSprint();

	UFUNCTION()
	void StopSprint();

	void SetState(EState State);

	UFUNCTION()
	void OnClickBegin();

	UFUNCTION()
	void OnClickEnd();

	void AttackBegin();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void OnAttackSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
									const FHitResult& SweepResult);

	UFUNCTION()
	void OnAttackSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void DeadEnd();

	void UpdateTarget();

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame();

	void TogglePauseUI();
};
