// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"


void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerControler not possesing a tank"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerControler possesing: %s"), *(ControlledTank->GetName()));
	}
}


ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank())
	{
		return;
	}

	FVector HitLocation; //Out paramater
	if (GetSightRayHitLocation(HitLocation))
	{

		GetControlledTank()->AimAt(HitLocation);
	}
}

//Get world location if linetrace through chrossair , true if hit landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	//find the chrosshair position in pixel coordinates.
	int32 ViewPortSizeX, ViewPortSizeY;
	GetViewportSize(ViewPortSizeX, ViewPortSizeY);
	auto ScreenLocation = FVector2D(ViewPortSizeX * CrossHairXLocation, ViewPortSizeY * CrossHairYLocation);

	//De-project the screen position of the crosshair to a world direction
	FVector LookDirection;
	if (GetLookLocation(ScreenLocation, LookDirection))
	{
		//line-trace along that look direction, and see what we hit (up to a max range)
		GetlookVectorHitLocation(LookDirection, HitLocation);
	}

	
	return true;
}

bool ATankPlayerController::GetLookLocation(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation; // to be discarded
	return DeprojectScreenPositionToWorld(ScreenLocation.X, 
		ScreenLocation.Y, 
		CameraWorldLocation, 
		LookDirection);
}

bool ATankPlayerController::GetlookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
	if (GetWorld()->LineTraceSingleByChannel(
			HitResult, 
			StartLocation, 
			EndLocation, 
			ECollisionChannel::ECC_Visibility))
	{
			HitLocation = HitResult.Location;
			return true;
	}
		HitLocation = FVector(0);
		return false; // line traces didnt succeed 
}