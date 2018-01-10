// Copyright Alex DuPree 2017

#pragma once

#include "Gameframework/Actor.h"
#include "Engine/TriggerVolume.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlickerLights.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLightIntensity);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UFlickerLights : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFlickerLights();

	UPROPERTY(BlueprintAssignable)
	FLightIntensity LightFlicker;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
