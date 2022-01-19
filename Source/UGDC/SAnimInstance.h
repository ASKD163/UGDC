// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UGDC_API USAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UPROPERTY()
	APawn* Pawn;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class ASCharacter* SCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float Velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsJump;
	
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties();
};
