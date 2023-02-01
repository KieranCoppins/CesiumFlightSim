// Fill out your copyright notice in the Description page of Project Settings.


#include "FlightPlane.h"

void AFlightPlane::MoveForward(const float value)
{
	if (value != 0) {
		thrust += value * (MaxThrust / 60.f);
		thrust = FMath::Clamp(thrust, 0.f, MaxThrust);
	}
}

void AFlightPlane::RotateRight(const float value)
{
	float currentControlL = ControlSurfaces[PlaneControlSurface::LeftWing].GetControl();
	float currentControlR = ControlSurfaces[PlaneControlSurface::RightWing].GetControl();
	ControlSurfaces[PlaneControlSurface::LeftWing].SetControl(currentControlL + (value));
	ControlSurfaces[PlaneControlSurface::RightWing].SetControl(currentControlR + (value * -1.f));
}

void AFlightPlane::RotateUp(const float value)
{
	ControlSurfaces[PlaneControlSurface::LeftWing].SetControl(value);
	ControlSurfaces[PlaneControlSurface::RightWing].SetControl(value);

	ControlSurfaces[PlaneControlSurface::Tailplane].SetControl(value * -1.f);
}

void AFlightPlane::RudderRight(const float value)
{
	ControlSurfaces[PlaneControlSurface::Rudder].SetControl(value);
}

void AFlightPlane::MoveCameraUp(const float value)
{
	FRotator relativeRotation = SpringArm->GetRelativeRotation();
	relativeRotation.Pitch += value;
	SpringArm->SetRelativeRotation(relativeRotation);
}

void AFlightPlane::MoveCameraRight(const float value)
{
	FRotator relativeRotation = SpringArm->GetRelativeRotation();
	relativeRotation.Yaw += value;
	SpringArm->SetRelativeRotation(relativeRotation);
}

// Sets default values
AFlightPlane::AFlightPlane()
{
	// Create our components
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 10000.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	GlobalAnchor = CreateDefaultSubobject<UCesiumGlobeAnchorComponent>(TEXT("Global Anchor"));

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFlightPlane::BeginPlay()
{
	Super::BeginPlay();

	FVector ActorScalar = GetActorScale();

	// Create our control surfaces
	ControlSurfaces.Add(PlaneControlSurface::LeftWing, ControlSurface(
		FMatrix(
		FVector(-0.1f, 0, 0),
		FVector(0, -0.1f, 1),
		FVector(0, 0, -0.5f),
		FVector(1, 1, 1)
	),
		FMatrix(
		FVector(-0.2f, 0, 0),
		FVector(0, -0.1f, -0.2f),
		FVector(0, 0, -0.5f),
		FVector(1, 1, 1)
	),
		FMatrix(
		FVector(-0.2f, 0, 0),
		FVector(0, -0.1f, 1.4f),
		FVector(0, 0.f, -0.5f),
		FVector(1, 1, 1)
	),
		FVector(-400, -32.5f, 0) * ActorScalar
	));

	ControlSurfaces.Add(PlaneControlSurface::RightWing, ControlSurface(
		FMatrix(
		FVector(-0.1f, 0, 0),
		FVector(0, -0.1f, 1),
		FVector(0, 0, -0.5f),
		FVector(1, 1, 1)
	),
		FMatrix(
		FVector(-0.2f, 0, 0),
		FVector(0, -0.1f, -0.2f),
		FVector(0, 0, -0.5f),
		FVector(1, 1, 1)
	),
		FMatrix(
		FVector(-0.2f, 0, 0),
		FVector(0, -0.1f, 1.4f),
		FVector(0, 0.f, -0.5f),
		FVector(1, 1, 1)
	),
		FVector(400, -32.5f, 0) * ActorScalar
	));

	ControlSurfaces.Add(PlaneControlSurface::Tailplane, ControlSurface(
		FMatrix(
		FVector(-0.1f, 0, 0),
		FVector(0, -0.1f, 0),
		FVector(0, 0, -0.f),
		FVector(1, 1, 1)
	),
		FMatrix(
		FVector(-0.1f, 0, 0),
		FVector(0, -0.1f, -1),
		FVector(0, 0, -0.f),
		FVector(1, 1, 1)
	),
		FMatrix(
		FVector(-0.1f, 0, 0),
		FVector(0, -0.1f, 1),
		FVector(0, 0, -0.f),
		FVector(1, 1, 1)
	),
		FVector(0, -550, 0) * ActorScalar
	));

	ControlSurfaces.Add(PlaneControlSurface::Rudder, ControlSurface(
		FMatrix(
		FVector(-0.5f, 0, 0),
		FVector(0, -0.1f, 0),
		FVector(0, 0, -0.f),
		FVector(1, 1, 1)
	),
		FMatrix(
		FVector(-0.5f, 0, 0),
		FVector(-0.4f, -0.1f, 0),
		FVector(0, 0, -0.f),
		FVector(1, 1, 1)
	),
		FMatrix(
		FVector(-0.5f, 0, 0),
		FVector(0.4f, -0.1f, 0),
		FVector(0, 0, -0.f),
		FVector(1, 1, 1)
	),
		FVector(0, -550, 0) * ActorScalar
	));
	
}

// Called every frame
void AFlightPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(Mesh->GetComponentLocation());
	UE_LOG(LogTemp, Display, TEXT("Current Velocity: %s"), *Mesh->GetComponentVelocity().ToString());
	UE_LOG(LogTemp, Display, TEXT("Thrust: %f"), thrust);
	Mesh->AddForce(Mesh->GetRightVector() * thrust * Mesh->CalculateMass());
	for (TTuple<PlaneControlSurface, ControlSurface> cSurface: ControlSurfaces)
	{
		cSurface.Value.UpdateForce(Mesh, 0.f);
	}

	UE_LOG(LogTemp, Display, TEXT("Left Wing control setting: %f"), ControlSurfaces[PlaneControlSurface::LeftWing].GetControl());
	UE_LOG(LogTemp, Display, TEXT("Right Wing control setting: %f"), ControlSurfaces[PlaneControlSurface::RightWing].GetControl());
	UE_LOG(LogTemp, Display, TEXT("Rudder control setting: %f"), ControlSurfaces[PlaneControlSurface::Rudder].GetControl());
	UE_LOG(LogTemp, Display, TEXT("Tail plane control setting: %f"), ControlSurfaces[PlaneControlSurface::Tailplane].GetControl());
}

// Called to bind functionality to input
void AFlightPlane::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("ThrustUp", this, &AFlightPlane::MoveForward);
	PlayerInputComponent->BindAxis("NoseUp", this, &AFlightPlane::RotateUp);
	PlayerInputComponent->BindAxis("Roll", this, &AFlightPlane::RotateRight);
	PlayerInputComponent->BindAxis("Rudder", this, &AFlightPlane::RudderRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AFlightPlane::MoveCameraUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AFlightPlane::MoveCameraRight);


}

