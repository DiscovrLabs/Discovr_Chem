// Fill out your copyright notice in the Description page of Project Settings.

#include "ChemistryFoundations.h"
#include "ChallengeButton.h"


// Sets default values
AChallengeButton::AChallengeButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	//ButtonMesh->SetWorldScale3D(FVector(0.25f, 0.25f, 0.25f));
	RootComponent = ButtonMesh;
}

// Called when the game starts or when spawned
void AChallengeButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChallengeButton::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

