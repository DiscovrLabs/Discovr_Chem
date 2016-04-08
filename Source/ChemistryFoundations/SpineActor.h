// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "SpineActor.generated.h"

UCLASS()
class CHEMISTRYFOUNDATIONS_API ASplineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
		USplineComponent* SplineComponent;
	
};
