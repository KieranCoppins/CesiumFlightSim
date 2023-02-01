// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControlSurface.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"

#include "CesiumGlobeAnchorComponent.h"

#include "FlightPlane.generated.h"

UCLASS()
class CESIUMFLIGHTSIM_API AFlightPlane : public APawn
{
	GENERATED_BODY()

	enum class PlaneControlSurface : uint8 {
		LeftWing,
		RightWing,
		Rudder,
		Tailplane,
	};

	TMap<PlaneControlSurface, ControlSurface> ControlSurfaces;

	// Components
	UPROPERTY(EditAnywhere, Category = "Components")
		USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Components")
		USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category = "Components")
		UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Components")
		UCesiumGlobeAnchorComponent* GlobalAnchor;

	// Plane parameters

	UPROPERTY(EditAnywhere, Category = "Plane Parameters")
		float MaxThrust;

	UPROPERTY(EditAnywhere, Category = "Plane Parameters")
		float RotateSpeed;

	UPROPERTY(EditAnywhere, Category = "Plane Parameters")
		float LiftModifier;

	// Private Functions
	void MoveForward(const float value);
	void RotateRight(const float value);
	void RotateUp(const float value);
	void RudderRight(const float value);

	void MoveCameraUp(const float value);
	void MoveCameraRight(const float value);

	// Private Variables
	float thrust;


public:
	// Sets default values for this pawn's properties
	AFlightPlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
