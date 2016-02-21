// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SubAtomic_Particle.h"
#include "StructCollection.h"
#include "Electron.generated.h"

/**
 * 
 */
UCLASS()
class CHEMISTRYFOUNDATIONS_API AElectron : public ASubAtomic_Particle
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AElectron();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called from outside this class to start movement
	UFUNCTION(BlueprintCallable, Category = "Movement")
		void MoveToNode();

		void Reset();

	UFUNCTION(BlueprintCallable, Category = "Movement")
		virtual void Move(FVector TargetObject, FVector TargetOffset, FRotator TargetRot = FRotator(0,0,0)) override;

	void SetNode(USceneComponent* NewNode);

	APawn* Player;
	FAtom*  MyAtom;
	bool Spawned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		USceneComponent* Node;

protected:
	bool Following;
	bool ShouldMoveVector;
	bool ResetPos;

	FVector SpawnPoint;
};
