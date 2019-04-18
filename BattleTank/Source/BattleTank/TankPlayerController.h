// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tank.h"
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h" //must be the last include

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	ATank* GetControlledTank() const;

	virtual void BeginPlay() override;

	//start the tank moving the barrel so that a shot would hit where
	//the crosshair would hit the world
	void AimTowardsCrosshair();

	virtual void Tick(float DeltaTime) override;
	
	//return on OUT paramiter, true if hit landscape
	bool GetSightRayHitLocation(FVector& OutHitLocation) const;

	bool GetLookLocation(FVector2D ScreenLocation, FVector & LookDirection) const;

	bool GetlookVectorHitLocation(FVector LookDirection, FVector & HitLocation) const;

	UPROPERTY(EditAnywhere)
		float CrossHairXLocation = .5f;

	UPROPERTY(EditAnywhere)
		float CrossHairYLocation = .33333f;

	UPROPERTY(EditAnywhere)
		float LineTraceRange = 1000000;

};
