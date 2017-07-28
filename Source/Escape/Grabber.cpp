// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    FVector PlayerViewLocation;
    FRotator PlayerViewRotation;

    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                OUT PlayerViewLocation,
                OUT PlayerViewRotation
                );

    UE_LOG(LogTemp, Warning, TEXT("Grabber location = %s, rotation = %s"), *PlayerViewLocation.ToString(), *PlayerViewRotation.ToString());

    // draw debug line
    FVector LineTraceEnd = PlayerViewLocation + PlayerViewRotation.Vector() * Reach;

    UKismetSystemLibrary::DrawDebugLine(
                GetWorld(),
                PlayerViewLocation,
                LineTraceEnd,
                FColor(255, 0, 0),
                0.f, // duration
                3.f // thickness
    );


}

