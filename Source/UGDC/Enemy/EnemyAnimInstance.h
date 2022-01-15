// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UGDC_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UPROPERTY()
	APawn* Pawn;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class AEnemy* Enemy;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float Velocity;

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties();
	
};
