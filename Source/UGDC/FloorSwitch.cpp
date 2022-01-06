// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitch.h"

#include "Components/BoxComponent.h"

// Sets default values
AFloorSwitch::AFloorSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerBox->SetBoxExtent(FVector(64,64,32));

	FloorSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Switch"));
	FloorSwitch->SetupAttachment(RootComponent);

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(RootComponent);

	DoorLocation = FVector::ZeroVector;
	SwitchLocation = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorSwitch::HandleOnComponentBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AFloorSwitch::HandleOnComponentEndOverlap);

	DoorLocation = Door->GetComponentLocation();
	SwitchLocation = FloorSwitch->GetComponentLocation();
}

// Called every frame
void AFloorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorSwitch::HandleOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("BeginOverlap"));

	RaiseDoor();
	LowerFloorSwitch();
	GetWorldTimerManager().ClearTimer(DoorTimerHandle);
	//
}

void AFloorSwitch::HandleOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Error, TEXT("EndOverlap"));
	//LowerDoor();
	//RaiseFloorSwitch();

	GetWorldTimerManager().SetTimer(DoorTimerHandle, this, &AFloorSwitch::CloseDoor, 1.f, false, -1.f);
}

void AFloorSwitch::UpdateDoorLocation(float Z)
{
	//UE_LOG(LogTemp, Log, TEXT("Door %.1f"), Z);

	FVector NewLocation = DoorLocation;
	NewLocation.Z += Z;
	Door->SetWorldLocation(NewLocation);
}

void AFloorSwitch::UpdateSwitchLocation(float Z)
{
	//UE_LOG(LogTemp, Log, TEXT("Switch %.1f"), Z);

	FVector NewLocation = SwitchLocation;
	NewLocation.Z += Z;
	FloorSwitch->SetWorldLocation(NewLocation);
}

void AFloorSwitch::CloseDoor()
{
	LowerDoor();
	RaiseFloorSwitch();
}