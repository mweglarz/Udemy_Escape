// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"

#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor() {
    // Set this component to be initialized when the game starts, and to be ticked
    // every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}

// Called when the game starts
void UOpenDoor::BeginPlay() {
	Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("Open door begin play"));
    Owner = GetOwner();
    InitialDoorAngleOffset = Owner->GetActorRotation().Yaw;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType,
                              FActorComponentTickFunction *ThisTickFunction) {

    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (GetTotalMassOfActorsOnPlate() > 21.f) { // TODO treshhold mass to variable
        OpenTheDoor();
        LastDoorOpenTime = GetWorld()->GetTimeSeconds();
    }

    if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
        CloseTheDoor();
    }
}

void UOpenDoor::OpenTheDoor() {
    if (!Owner) return;
    Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseTheDoor() {
    if (!Owner) return;
    Owner->SetActorRotation(FRotator(0.f, InitialDoorAngleOffset, 0.f));
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {

    float TotalMass = 0.f;
    // TODO iterate through all overlaping actors and sum their masses
    TArray<AActor*> ActorsOnPlate;
    if (!PressurePlate) return TotalMass;
    PressurePlate->GetOverlappingActors(OUT ActorsOnPlate);

    for (const auto* Actor: ActorsOnPlate) {
       TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
    }
    UE_LOG(LogTemp, Warning, TEXT("Total mass on plate = %.2f"), TotalMass);

    return TotalMass;
}
