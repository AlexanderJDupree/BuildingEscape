// Copyright Alex DuPree 2017

#include "Grabber.h"
#include "Components/ActorComponent.h"
#include "Public/DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

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
	FindPhysicsHandleComponent();
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!PhysicsHandle) { return; }
	if (bDrawDebugLine) { CreateDebugLine(); }
	// Checks if physics handle has grabbed an object, sets transform values of grabbed object to the linetrace end
	if (PhysicsHandle->GrabbedComponent){ 
		PhysicsHandle->SetTargetLocationAndRotation(CalculateLineTraceEnd(), GetOwner()->GetActorRotation());
	}
}
//Debug tool for visualizing reach
void UGrabber::CreateDebugLine()
{
	/*UE_LOG(LogTemp, Warning, TEXT("Location: %s Rotation:%s"),
	*PlayerViewLocation.ToString(),
	*PlayerViewRotation.ToString());*/

	PlayerLocationRotation Player = CalculateLineTraceStart();
	DrawDebugLine(
		GetWorld(),
		Player.PlayerViewLocation,
		CalculateLineTraceEnd(),
		FColor(255, 0, 0),
		false,
		0.0f,
		0.0f,
		10.0f
	);
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR: No Physics Handle found on %s"), *GetOwner()->GetName())
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
		InputComponent->BindAction("Throw", IE_Pressed, this, &UGrabber::Throw);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR: No input component found on %s"), *GetOwner()->GetName())
	}
}

void UGrabber::Grab()
{
	// Try and reach any actors with physics body collision channel set
	FHitResult HitResult =  GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();
	// If we hit something then attach a physics handle
	if (ActorHit != nullptr)
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, // no Bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
}

void UGrabber::Release() 
{ 
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent(); 
}

void UGrabber::Throw()
{
	PlayerLocationRotation Player = CalculateLineTraceStart();
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	AActor* ActorHit = HitResult.GetActor();
	FVector ImpulseVector = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetActorForwardVector();
	if (!ActorHit) { return; }
	UPrimitiveComponent* ObjectThrown = ActorHit->FindComponentByClass<UPrimitiveComponent>();
	if (ObjectThrown)
	{
		Release();
		ObjectThrown->AddImpulse(ImpulseVector * ThrowStrength, NAME_None, true);
	}
}

FVector UGrabber::CalculateLineTraceEnd()
{
	PlayerLocationRotation Player = CalculateLineTraceStart();
	return Player.PlayerViewLocation + Player.PlayerViewRotation.Vector() * Reach;
}

PlayerLocationRotation UGrabber::CalculateLineTraceStart()
{
	PlayerLocationRotation playerLocationRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerLocationRotation.PlayerViewLocation,
		OUT playerLocationRotation.PlayerViewRotation
	);
	return playerLocationRotation;
}


const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	PlayerLocationRotation Player = CalculateLineTraceStart();
	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	/// Line-Trace (raycast)
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		Player.PlayerViewLocation,
		CalculateLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	
	return HitResult;
}

