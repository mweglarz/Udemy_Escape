// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber() {
	PrimaryComponentTick.bCanEverTick = true;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {

    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    FVector LineTraceEnd = std::get<1>(GetGrabVector());
    if (!PhysicsHandle) return;
    if (PhysicsHandle->GrabbedComponent) {
        // move the object that we're holding
        PhysicsHandle->SetTargetLocation(LineTraceEnd);
    }
}

void UGrabber::Grab() {

    /// Try and reach any actors with physics body collision channel set
    FHitResult Hit = GetPhysicsBodyInReach();

    UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();
    AActor* ActorHit = Hit.GetActor();

    if (!ActorHit) return; // guard if ActorHit not nullptr
    // attach physics handle
    if (!PhysicsHandle) return;
    PhysicsHandle->GrabComponent(ComponentToGrab,
                                 NAME_None,
                                 ComponentToGrab->GetOwner()->GetActorLocation(),
                                 true // allow rotation
                                 );
}

void UGrabber::Release() {
    if (!PhysicsHandle) return;
    PhysicsHandle->ReleaseComponent();
}

// Called when the game starts
void UGrabber::BeginPlay() {
  Super::BeginPlay();

  FindAndAttachPhysicsHandle();
  FindAndBindInputComponent();
}

void UGrabber::FindAndAttachPhysicsHandle() {
    /// Look for attached Physics Handle
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

    if (!PhysicsHandle) {
        UE_LOG(LogTemp, Error, TEXT("Cannot find PhysicsHandle in object = %s"), *(GetOwner()->GetName()))
    }
}

void UGrabber::FindAndBindInputComponent() {
    // Look for attached InputComponent (only appears in runtime)
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

    if (InputComponent) {
        /// Bind the input axis
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);

    } else {
        UE_LOG(LogTemp, Error, TEXT("Cannot find InputComponent for object %s"), *(GetOwner()->GetName()))
    }
}

const FHitResult UGrabber::GetPhysicsBodyInReach() {

    MWFGrabVector GrabVector = GetGrabVector();

    /// Line-trace out to reach distance
    FHitResult Hit;
    GetWorld()->LineTraceSingleByObjectType(
                OUT Hit,
                std::get<0>(GrabVector),
                std::get<1>(GrabVector),
                FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
                );

    return Hit;
}

const MWFGrabVector UGrabber::GetGrabVector() const {
    FVector PlayerViewLocation;
    FRotator PlayerViewRotation;

    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                OUT PlayerViewLocation,
                OUT PlayerViewRotation
                );

    FVector LineTraceEnd = PlayerViewLocation + PlayerViewRotation.Vector() * Reach;

    return std::make_tuple(PlayerViewLocation, LineTraceEnd);
}

