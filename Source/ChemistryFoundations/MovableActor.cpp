// Fill out your copyright notice in the Description page of Project Settings.

#include "ChemistryFoundations.h"
#include "MovableActor.h"


void AMovableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->ShouldMove)
	{

		FVector TempTarget;

		if (Interpolate)
			TempTarget = FMath::VInterpTo(this->GetActorLocation(), Target, DeltaTime, 0.5f);
		else
			TempTarget = FMath::VInterpConstantTo(this->GetActorLocation(), Target, DeltaTime, 40.f);

		this->SetActorLocation(TempTarget);

		if ((this->GetActorLocation() - Target).Size() < 1)
		{
			this->ShouldMove = false;
		}
	}

	if (TargetRotation != FRotator::ZeroRotator)
	{
		if (this->ShouldRotate)
		{
			FRotator TempRot = FMath::RInterpConstantTo(this->GetActorRotation(), TargetRotation, DeltaTime, 18.f);
			this->SetActorRotation(TempRot);

			if (this->GetActorRotation().Equals(TargetRotation))
				this->ShouldRotate = false;
		}
	}
}