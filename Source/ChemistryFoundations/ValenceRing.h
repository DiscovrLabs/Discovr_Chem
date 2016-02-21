// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "ValenceRing.generated.h"

/**
 * 
 */
UCLASS()
class CHEMISTRYFOUNDATIONS_API AValenceRing : public AActor
{
	GENERATED_BODY()

public:
	AValenceRing();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		URotatingMovementComponent* RotatingMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<USceneComponent*> Nodes;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	void SetTargetRotationRate(float NewRate);
	void SpawnNodes(int NodeCount);

protected:
	bool ShouldChange;
	float TargetRotationRate;
	float radius;
};
