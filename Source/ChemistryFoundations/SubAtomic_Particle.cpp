// Fill out your copyright notice in the Description page of Project Settings.

#include "ChemistryFoundations.h"
#include "SubAtomic_Particle.h"


// Sets default values
ASubAtomic_Particle::ASubAtomic_Particle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/Meshes/Particle.Particle'"));

	if (StaticMesh.Succeeded())
	{
		ParticleVisualComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParticleMesh"));
		ParticleVisualComponent->SetStaticMesh(StaticMesh.Object);
		ParticleVisualComponent->SetWorldScale3D(FVector(0.25f, 0.25f, 0.25f));
		ParticleVisualComponent->AttachTo(Root);
	}

	Clicked = false;
	ShouldMove = false;
}

// Called when the game starts or when spawned
void ASubAtomic_Particle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASubAtomic_Particle::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called from outside this class to start movement
void ASubAtomic_Particle::Move(FVector TargetObject, FVector TargetOffset, FRotator TargetRot)
{
	Target = TargetObject + TargetOffset;
	TargetRotation = TargetRot;
	ShouldRotate = true;
	ShouldMove = true;
	Clicked = true;
}

// Called from outside this class to start movement
void ASubAtomic_Particle::Move(FVector TargetObject, FRotator TargetAngle, FVector LocalOffset)
{
}