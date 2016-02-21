// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SubAtomic_Particle.h"
#include "Neutron.generated.h"

/**
 * 
 */
UCLASS()
class CHEMISTRYFOUNDATIONS_API ANeutron : public ASubAtomic_Particle
{
	GENERATED_BODY()
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	
	
};
