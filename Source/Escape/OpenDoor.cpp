// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("Open door begin play"));
    TriggerActor = GetWorld()->GetFirstPlayerController()->GetPawn();
    Owner = GetOwner();
    InitialDoorAngleOffset = Owner->GetActorRotation().Yaw;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (PressurePlate->IsOverlappingActor(TriggerActor)) {
        OpenTheDoor();
        LastDoorOpenTime = GetWorld()->GetTimeSeconds();
    }

    // check if its time to close the door
    if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
        CloseTheDoor();
    }
}

void UOpenDoor::OpenTheDoor() {
    Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseTheDoor() {
    Owner->SetActorRotation(FRotator(0.f, InitialDoorAngleOffset, 0.f));
}
