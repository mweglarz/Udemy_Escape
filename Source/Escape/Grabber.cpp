// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

    FindPhysicsHandle();
    FindInputComponent();
}

void UGrabber::Grab() {
    UE_LOG(LogTemp, Warning, TEXT("Grab delegate called!"))
}

void UGrabber::Release() {
    UE_LOG(LogTemp, Warning, TEXT("Release delegate called!"))
}

void UGrabber::FindInputComponent() {
    // Look for attached InputComponent (only appears in runtime)
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

    if (InputComponent) {
        UE_LOG(LogTemp, Warning, TEXT("Input component found!"))

        /// Bind the input axis
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);

    } else {
        UE_LOG(LogTemp, Error, TEXT("Cannot find InputComponent for object %s"), *(GetOwner()->GetName()))
    }
}

void UGrabber::FindPhysicsHandle() {
    /// Look for attached Physics Handle
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

    if (PhysicsHandle) {
        UE_LOG(LogTemp, Warning, TEXT("Physics handle found!"))

    } else {
        UE_LOG(LogTemp, Error, TEXT("Cannot find PhysicsHandle in object = %s"), *(GetOwner()->GetName()))
    }
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

//    UE_LOG(LogTemp, Warning, TEXT("Grabber location = %s, rotation = %s"), *PlayerViewLocation.ToString(), *PlayerViewRotation.ToString());

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

    /// Line-trace out to reach distance
    FHitResult Hit;
    FCollisionQueryParams QueryParams = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());

    GetWorld()->LineTraceSingleByObjectType(
                OUT Hit,
                PlayerViewLocation,
                LineTraceEnd,
                FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                QueryParams);

    /// check what we hit
    AActor* ActorHit = Hit.GetActor();
    if (ActorHit) {
        UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
    }
}

