// Fill out your copyright notice in the Description page of Project Settings.

#include "ChemistryFoundations.h"
#include "SpawnVolume.h"


// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	RootComponent = WhereToSpawn;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawnVolume::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ASpawnVolume::SpawnTable()
{
	GetWorld()->SpawnActor<AActor>(PeriodicTableBP, FVector(-274, -2500, 1128), FRotator(0, 90, 0));
}

void ASpawnVolume::SpawnParticles(int32 GameMode)
{
	UWorld* ThisWorld = this->GetWorld();
	switch (GameMode)
	{
		//Tutorial Mode
	case 0:
		//Spawn 2 Protons, 2 Neutrons, and 2 Electrons
		for (int i = 0; i < 2; i++)
		{
			AProton* TempProton = ThisWorld->SpawnActor<AProton>(ProtonBP, GetRandomPointInVolume(), FRotator::ZeroRotator);
			ANeutron* TempNeutron = ThisWorld->SpawnActor<ANeutron>(NeutronBP, GetRandomPointInVolume(), FRotator::ZeroRotator);
			AElectron* TempElectron = ThisWorld->SpawnActor<AElectron>(ElectronBP, GetRandomPointInVolume(), FRotator::ZeroRotator);

			SpawnedParticles.Add(TempProton);
			SpawnedParticles.Add(TempNeutron);
			SpawnedParticles.Add(TempElectron);
		}
		break;
		//Challenge Mode
	case 1:
		//Spawn 15 Electrons
		for (int i = 0; i < 15; i++)
		{
			AElectron* TempElectron = ThisWorld->SpawnActor<AElectron>(ElectronBP, GetRandomPointInVolume(), FRotator::ZeroRotator);
			SpawnedParticles.Add(TempElectron);
		}
		break;
	}
}

FVector ASpawnVolume::GetRandomPointInVolume()
{
	FVector RandomLocation;
	float MinX, MinY, MinZ;
	float MaxX, MaxY, MaxZ;

	FVector Origin;
	FVector BoxExtent;

	//Get the spawn volume's origin and box extent
	Origin = WhereToSpawn->Bounds.Origin;
	BoxExtent = WhereToSpawn->Bounds.BoxExtent;

	//Calculate the minimum X, Y, and Z
	MinX = Origin.X - BoxExtent.X / 2.f;
	MinY = Origin.Y - BoxExtent.Y / 2.f;
	MinZ = Origin.Z - BoxExtent.Z / 2.f;

	//Calculate the maximum X, Y, and Z
	MaxX = Origin.X + BoxExtent.X / 2.f;
	MaxY = Origin.Y + BoxExtent.Y / 2.f;
	MaxZ = Origin.Z + BoxExtent.Z / 2.f;

	//The random spawn location will fall between the min X, Y, and Z
	RandomLocation.X = FMath::FRandRange(MinX, MaxX);
	RandomLocation.Y = FMath::FRandRange(MinY, MaxY);
	RandomLocation.Z = FMath::FRandRange(MinZ, MaxZ);

	return RandomLocation;
}

void ASpawnVolume::DeleteParticles()
{
	for (AActor* Particle : SpawnedParticles)
	{
		if (Particle != NULL)
			Particle->Destroy();
	}
	SpawnedParticles.Empty();
}