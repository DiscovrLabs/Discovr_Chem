// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Electron.h"
#include "Proton.h"
#include "Neutron.h"
#include "ValenceRing.h"
#include "ParticleContainer.generated.h"

USTRUCT()
struct FParticleContainer_
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<AProton*> Protons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<ANeutron*> Neutrons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<AElectron*> Electrons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<AValenceRing*> ValenceRings;
};

class CHEMISTRYFOUNDATIONS_API ParticleContainer
{
public:
	ParticleContainer();
	~ParticleContainer();
};
