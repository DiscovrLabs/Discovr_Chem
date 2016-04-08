// Fill out your copyright notice in the Description page of Project Settings.

#include "ChemistryFoundations.h"
#include "PAL.h"


// Sets default values
APAL::APAL()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;

	SplineDistance = 0.0f;
	bStarted = false;
}

// Called when the game starts or when spawned
void APAL::BeginPlay()
{
	Super::BeginPlay();

	SplineComponent = SplineActor->SplineComponent;
	LocationLastTick = GetActorLocation();
}

// Called every frame
void APAL::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	SetActorLocation(SplineComponent->GetLocationAtDistanceAlongSpline(SplineDistance, ESplineCoordinateSpace::World));
	SetActorRotation(FRotationMatrix::MakeFromX(LookAtPoint - GetActorLocation()).Rotator());
	SplineDistance += Speed * DeltaTime;
	if (bStarted)
	{
		if (GetActorLocation().Equals(LocationLastTick, 0.2f))
		{
			SplineDistance = 0.0f;
			bStarted = false;
		}
		else
		{
			LocationLastTick = GetActorLocation();
		}
	}
	else
	{
		bStarted = true;
		LocationLastTick = GetActorLocation();
	}
}

void APAL::UpdateAudioInfo(USoundBase * _Clip)
{
	Clip = _Clip;
	AudioStartTime = GetWorld()->TimeSeconds;
}

