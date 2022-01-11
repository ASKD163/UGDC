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
	
public:	
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

};
