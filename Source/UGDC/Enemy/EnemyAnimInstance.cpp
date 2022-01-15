// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

#include "Enemy.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();
		AEnemy* Tmp = Cast<AEnemy>(Pawn);
		if (Tmp) Enemy = Tmp;
	}
}

void UEnemyAnimInstance::UpdateAnimationProperties()
{
	if (!Pawn) Pawn = TryGetPawnOwner();

	if (Pawn)
	{
		FVector VelocityVector = Pawn->GetVelocity();
		VelocityVector.Z = 0;
		Velocity = VelocityVector.Size();
	}
}
