// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Molecule.generated.h"

UCLASS()
class CHEMISTRYFOUNDATIONS_API AMolecule : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMolecule();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called from outside this class to start movement
	virtual void Move(FVector TargetObject, FVector TargetOffset);

	// StaticMeshComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UStaticMeshComponent* ParticleVisualComponent;

protected:
	bool ShouldMove;
	FVector Target;
};
