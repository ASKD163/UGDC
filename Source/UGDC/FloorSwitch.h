// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorSwitch.generated.h"

UCLASS()
class UGDC_API AFloorSwitch : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Component")
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	class UStaticMeshComponent* FloorSwitch;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	class UStaticMeshComponent* Door;

	FTimerHandle DoorTimerHandle;
	
public:	
	// Sets default values for this actor's properties
	AFloorSwitch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	FVector DoorLocation;

	UPROPERTY(BlueprintReadWrite)
	FVector SwitchLocation;

	UFUNCTION()
	void HandleOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                   const FHitResult& SweepResult);

	UFUNCTION()
	void HandleOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void RaiseFloorSwitch();

	UFUNCTION(BlueprintImplementableEvent)
	void LowerFloorSwitch();

	UFUNCTION(BlueprintImplementableEvent)
	void RaiseDoor();

	UFUNCTION(BlueprintImplementableEvent)
	void LowerDoor();

	UFUNCTION(BlueprintCallable)
	void UpdateDoorLocation(float Z);

	UFUNCTION(BlueprintCallable)
	void UpdateSwitchLocation(float Z);

	UFUNCTION()
	void CloseDoor();
};
