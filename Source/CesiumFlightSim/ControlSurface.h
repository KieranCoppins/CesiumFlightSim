// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"

/**
 * 
 */
class CESIUMFLIGHTSIM_API ControlSurface
{
public:
	ControlSurface(const FMatrix& baseTensor, const FMatrix& minTensor, const FMatrix& maxTensor, const FVector& position);
	ControlSurface(const FMatrix& baseTensor, const FVector& position);
	~ControlSurface();

	void SetControl(float value);
	float GetControl() const;

	void UpdateForce(UStaticMeshComponent* mesh, float duration);

protected:

	FMatrix baseTensor;
	FMatrix minTensor;
	FMatrix maxTensor;

	FVector position;

	float controlSetting;

private:

	FMatrix GetTensor();

	FMatrix LinearInterpolate(const FMatrix& a, const FMatrix& b, float value);

};
