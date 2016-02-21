// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "ValenceRing.h"
#include "StructCollection.generated.h"

USTRUCT()
struct FAtom
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 ID; //Type of atom based purely on an arbitrary list

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 ProtonNum; //Number of protons in this atom

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 NeutronNum; //Number of neutrons in this atom

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 ElectronNum; //Number of electrons in this atom

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 IncomingElectrons; //Number of incoming electrons

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<FVector> ProtonOffsets; //Array of the protons offsets from the center of the atom

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<FVector> NeutronOffsets; //Array of the neutron offsets from the center of the atom

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
		TArray<AValenceRing*> ValenceRings; //Array of valence ring references

	FAtom()
	{
		ID = 0;
		ProtonNum = 0;
		NeutronNum = 0;
		ElectronNum = 0;
		IncomingElectrons = 0;
	}
};

class CHEMISTRYFOUNDATIONS_API StructCollection
{
public:
	StructCollection();
	~StructCollection();
};
