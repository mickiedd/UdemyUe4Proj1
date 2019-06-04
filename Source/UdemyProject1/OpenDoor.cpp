// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Classes/Components/PrimitiveComponent.h"

#define OUT

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

	if (ActorThatOpens) {
		ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	}
}

void UOpenDoor::CloseDoor()
{
	// Find the owning Actor
	AActor* Owner = GetOwner();

	// Create a rotator
	FRotator NewRotation = FRotator(0.f, 0.f, 0.f);

	// Set the door rotation
	Owner->SetActorRotation(NewRotation);

	// Print something
	//UE_LOG(LogTemp, Error, TEXT("Oh the door is closed."));
}

void UOpenDoor::OpenDoor()
{
	OnOpenRequest.Broadcast();
	UE_LOG(LogTemp, Error, TEXT("Open Door Event Sent."));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (GetTotalMassOfActorsOnPlate() > 30.f/*PressurePlate->IsOverlappingActor(ActorThatOpens)*/) {
		OpenDoor();
		DoorLastOpenTime = GetWorld()->GetTimeSeconds();
	}
	if (GetWorld()->GetTimeSeconds() - DoorLastOpenTime > DoorDelayTime) {
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	TArray<AActor*> OverLappingActors;
	if (PressurePlate) {
		PressurePlate->GetOverlappingActors(OUT OverLappingActors);
		for (auto& one : OverLappingActors) {
			TotalMass += one->FindComponentByClass<UPrimitiveComponent>()->GetMass();
			UE_LOG(LogTemp, Error, TEXT("%s"), *one->GetName());
		}
	}
	return TotalMass;
}

