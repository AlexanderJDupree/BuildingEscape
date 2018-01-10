// Copyright Alex DuPree 2017

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "Gameframework/Actor.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


struct PlayerLocationRotation
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	PlayerLocationRotation CalculateLineTraceStart();

private:

	UPROPERTY(EditAnywhere)
	float Reach = 100.0f;

	UPROPERTY(EditAnywhere)
	float ThrowStrength = 100.0f;

	UPROPERTY(EditAnywhere)
	bool bDrawDebugLine = false;

	// PhysicsHandle and InputComponent are pointed to correct component on begin play
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	void CreateDebugLine();

	// Find (assumed) attached physics handle
	void FindPhysicsHandleComponent();

	// Set up (assumed) attached input component
	void SetupInputComponent();

	// Ray-cast and grab what's in reach
	void Grab();
	void Release();
	void Throw();

	// Returns current end of reach line
	FVector CalculateLineTraceEnd();

	// Return Hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

};
