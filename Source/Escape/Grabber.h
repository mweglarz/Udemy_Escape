// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <tuple>
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Grabber.generated.h"

typedef std::tuple<FVector, FVector> MWFGrabVector;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UGrabber : public UActorComponent {
GENERATED_BODY()

public:
  // Sets default values for this component's properties
  UGrabber();

  // Called every frame
  virtual void
  TickComponent(float DeltaTime, ELevelTick TickType,
                FActorComponentTickFunction *ThisTickFunction) override;
  void Grab();    // grab delegate method
  void Release(); // release delegate method

protected:
  // Called when the game starts
  virtual void BeginPlay() override;

/// Private fields
private:
  // How far player can reach to grab sth
  float Reach = 100.f;

  UPhysicsHandleComponent *PhysicsHandle = nullptr;
  UInputComponent *InputComponent = nullptr;

/// Private methods
private:
  void FindAndAttachPhysicsHandle();
  void FindAndBindInputComponent();

  // Return hit for first physics body in reach
  const FHitResult GetPhysicsBodyInReach();
  const MWFGrabVector GetGrabVector() const;
};
