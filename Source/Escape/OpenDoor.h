// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "OpenDoor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();
    void OpenTheDoor();
    void CloseTheDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

    UPROPERTY(EditAnywhere)
    ATriggerVolume* PressurePlate = nullptr;
    AActor* Owner = nullptr;

    UPROPERTY(EditAnywhere)
    float DoorCloseDelay = 1.f;
    UPROPERTY(EditAnywhere)
    float OpenAngle = 90.f;
    float LastDoorOpenTime;
    float InitialDoorAngleOffset;

private:

    // Total mass of overlapping objects in kg.
    float GetTotalMassOfActorsOnPlate();
};
