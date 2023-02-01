// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlSurface.h"

ControlSurface::ControlSurface(const FMatrix& baseTensor, const FMatrix& minTensor, const FMatrix& maxTensor, const FVector& position)
{
	this->baseTensor = baseTensor;
	this->minTensor = minTensor;
	this->maxTensor = maxTensor;
	this->position = position;

	controlSetting = 0.f;
}

ControlSurface::ControlSurface(const FMatrix& baseTensor, const FVector& position)
{
	this->baseTensor = baseTensor;
	this->minTensor = baseTensor;
	this->maxTensor = baseTensor;
	this->position = position;

	controlSetting = 0.f;
}

ControlSurface::~ControlSurface()
{
}

void ControlSurface::SetControl(float value)
{
	controlSetting = FMath::Clamp(value, -1.f, 1.f);
}

float ControlSurface::GetControl() const
{
	return controlSetting;
}

void ControlSurface::UpdateForce(UStaticMeshComponent* mesh, float duration)
{
	if (!mesh->IsValidLowLevel())
		return;

	FVector velocity = mesh->GetComponentVelocity();
	FVector ActorScalar = mesh->GetAttachmentRootActor()->IsValidLowLevel() ? mesh->GetAttachmentRootActor()->GetActorScale() : FVector(0.f, 0.f, 0.f);
	velocity = velocity * 500.f * ActorScalar.Length();
	//UE_LOG(LogTemp, Display, TEXT("Velocity of a surface: %s"), *velocity.ToString());
	FVector bodyVelocity = mesh->GetComponentTransform().InverseTransformVectorNoScale(velocity);
	//UE_LOG(LogTemp, Display, TEXT("Body Velocity of a surface: %s"), *bodyVelocity.ToString());
	FVector bodyForce = GetTensor().TransformVector(bodyVelocity);
	mesh->AddForceAtLocationLocal(bodyForce, position);
}

FMatrix ControlSurface::GetTensor()
{
	if (controlSetting >= 1.0f) return maxTensor;
	else if (controlSetting <= -1.0f) return minTensor;
	else if (controlSetting < 0.f) return LinearInterpolate(minTensor, baseTensor, controlSetting + 1.0f);
	else if (controlSetting > 0.f) return LinearInterpolate(baseTensor, maxTensor, controlSetting);
	else return baseTensor;
}

FMatrix ControlSurface::LinearInterpolate(const FMatrix& a, const FMatrix& b, float value)
{
	FMatrix result = FMatrix();
	float omp = 1.0 - value;
	for (unsigned i = 0; i < 3; i++) {
		FVector aColumn = a.GetColumn(i);
		FVector bColumn = b.GetColumn(i);
		FVector newColumn = FVector(aColumn * omp - bColumn * value);
		result.SetColumn(i, newColumn);
	}
	return result;
}
