// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Proton.h"
#include "Neutron.h"
#include "Electron.h"
#include "SpawnVolume.generated.h"

UCLASS()
class CHEMISTRYFOUNDATIONS_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
		UBoxComponent* WhereToSpawn;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void SpawnParticles(int32 GameMode);
	void DeleteParticles();
	TArray<AActor*> SpawnedParticles;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprints")
		TSubclassOf<AProton> ProtonBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprints")
		TSubclassOf<ANeutron> NeutronBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprints")
		TSubclassOf<AElectron> ElectronBP;

	FVector GetRandomPointInVolume();
};
