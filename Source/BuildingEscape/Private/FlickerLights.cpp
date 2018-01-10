// Copyright Alex DuPree 2017

#include "FlickerLights.h"
#include "Engine/World.h"
#include "Gameframework/Actor.h"


// Sets default values for this component's properties
UFlickerLights::UFlickerLights()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UFlickerLights::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFlickerLights::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	LightFlicker.Broadcast();
}

