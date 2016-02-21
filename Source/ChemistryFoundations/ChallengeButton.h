// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ChallengeButton.generated.h"

UCLASS()
class CHEMISTRYFOUNDATIONS_API AChallengeButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChallengeButton();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// StaticMeshComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UStaticMeshComponent* ButtonMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Num;
};
