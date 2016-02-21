// Fill out your copyright notice in the Description page of Project Settings.

#include "ChemistryFoundations.h"
#include "ValenceRing.h"

AValenceRing::AValenceRing()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement"));
	RotatingMovementComponent->RotationRate = FRotator(0,180,0);

	PrimaryActorTick.bCanEverTick = true;
}

void AValenceRing::Tick(float DeltaTime)
{
	if (this->ShouldChange)
	{
		float tempTarget = FMath::FInterpConstantTo(RotatingMovementComponent->RotationRate.Yaw, TargetRotationRate, DeltaTime, 240.f);
		RotatingMovementComponent->RotationRate = FRotator(0, tempTarget, 0);
		if (tempTarget == TargetRotationRate)
		{
			this->ShouldChange = false;
		}
	}
}

void AValenceRing::SetTargetRotationRate(float NewRate)
{
	TargetRotationRate = NewRate;
	this->ShouldChange = true;
}

void AValenceRing::SpawnNodes(int NodeCount)
{
	radius = 80.0f;
	float angle = 360.0f / NodeCount;

	for (int i = 0; i < NodeCount; i++)
	{
		float xPos = radius * FMath::Cos((angle * (i + 1)) * PI / 180);
		float yPos = radius * FMath::Sin((angle * (i + 1)) * PI / 180);
		USceneComponent* Node = ConstructObject<USceneComponent>(USceneComponent::StaticClass());
		Node->AttachTo(RootComponent);
		Node->AddLocalOffset(FVector(xPos, yPos, 0));
		Nodes.Add(Node);
	}
}