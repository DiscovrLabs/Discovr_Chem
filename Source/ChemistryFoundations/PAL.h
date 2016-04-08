// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "SpineActor.h"
#include "PAL.generated.h"

UCLASS()
class CHEMISTRYFOUNDATIONS_API APAL : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APAL();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void UpdateAudioInfo(USoundBase* _Clip);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Movement")
		ASplineActor* SplineActor;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Movement")
		FVector LookAtPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		USoundBase* Clip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		float AudioStartTime;

	USplineComponent* SplineComponent;
	FVector LocationLastTick;
	float SplineDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float Speed;

	bool bStarted;
	
};
