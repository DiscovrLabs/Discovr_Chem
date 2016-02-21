// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SubAtomic_Particle.generated.h"

UCLASS()
class CHEMISTRYFOUNDATIONS_API ASubAtomic_Particle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASubAtomic_Particle();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called from outside this class to start movement
	UFUNCTION(BlueprintCallable, Category = "Movement")
		virtual void Move(FVector TargetObject, FVector TargetOffset, FRotator TargetRot = FRotator::ZeroRotator);

	virtual void Move(FVector TargetObject, FRotator TargetAngle, FVector LocalOffset);

	// DefaultRootComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root")
		USceneComponent* Root;

	// StaticMeshComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UStaticMeshComponent* ParticleVisualComponent;

	void ToggleMove();

	bool Clicked;

protected:

	bool ShouldMove;
	bool ShouldRotate;
	FVector Target;
	FRotator TargetRotation;
};
