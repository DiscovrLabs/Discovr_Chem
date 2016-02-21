// Fill out your copyright notice in the Description page of Project Settings.

#include "ChemistryFoundations.h"
#include "Molecule.h"


// Sets default values
AMolecule::AMolecule()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/Meshes/Particle.Particle'"));

	if (StaticMesh.Succeeded())
	{
		ParticleVisualComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParticleMesh"));
		ParticleVisualComponent->SetStaticMesh(StaticMesh.Object);
		ParticleVisualComponent->SetWorldScale3D(FVector(0.25f, 0.25f, 0.25f));
		RootComponent = ParticleVisualComponent;
	}
}

// Called when the game starts or when spawned
void AMolecule::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMolecule::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (this->ShouldMove)
	{
		FVector tempTarget = FMath::VInterpTo(this->GetActorLocation(), Target, DeltaTime, 2);
		this->SetActorLocation(tempTarget);

		if ((this->GetActorLocation() - Target).Size() < 1)
		{
			this->ShouldMove = false;
		}
	}
}

void AMolecule::Move(FVector TargetObject, FVector TargetOffset)
{
	Target = TargetObject + TargetOffset;
	ShouldMove = true;
}

