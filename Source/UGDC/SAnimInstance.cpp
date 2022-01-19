// Fill out your copyright notice in the Description page of Project Settings.


#include "SAnimInstance.h"

#include "SCharacter.h"
#include "GameFramework/PawnMovementComponent.h"

void USAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			ASCharacter* Tmp = Cast<ASCharacter>(Pawn);
			if (Tmp) SCharacter = Tmp;
		}
	}

	
}

void USAnimInstance::UpdateAnimationProperties()
{
	if (!Pawn) Pawn = TryGetPawnOwner();

	if (Pawn)
	{
		FVector VelocityVector = Pawn->GetVelocity();
		VelocityVector.Z = 0;
		Velocity = VelocityVector.Size();

		bIsJump = Pawn->GetMovementComponent()->IsFalling();
	}
}
