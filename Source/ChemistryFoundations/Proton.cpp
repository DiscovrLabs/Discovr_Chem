// Fill out your copyright notice in the Description page of Project Settings.

#include "ChemistryFoundations.h"
#include "Proton.h"

void AProton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->ShouldMove)
	{
		FVector tempTarget = FMath::VInterpTo(this->GetActorLocation(), Target, DeltaTime, 2);
		this->SetActorLocation(tempTarget);

		if ((this->GetActorLocation() - Target).Size() < 1)
		{
			this->ShouldMove = false;
			SetActorRotation(FRotator::ZeroRotator);
		}
	}
}