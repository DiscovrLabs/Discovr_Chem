// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Proton.h"
#include "Neutron.h"
#include "Electron.h"
#include "Molecule.h"
#include "SubAtomic_Particle.h"
#include "ValenceRing.h"
#include "StructCollection.h"
#include "ParticleContainer.h"
#include "ChallengeButton.h"
#include "SpawnVolume.h"
#include "ChemPlayer.generated.h"

#define FAST 360
#define NORMAL 180
#define SLOW 90

USTRUCT()
struct FMoleculeStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<int32> AtomIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<FVector> AtomOffsets;

	FMoleculeStruct()
	{
	}
};

UCLASS()
class CHEMISTRYFOUNDATIONS_API AChemPlayer : public APawn
{
	GENERATED_BODY()

public:
	/***********************************************************************************************************************************************
	* Basic Functions, Input, and Data Members
	***********************************************************************************************************************************************/
	AChemPlayer();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	void DisableActor(AActor* Actor, bool bDisable);

	UFUNCTION(BlueprintCallable, Category = "Raycast")
		void Raycast();

	UFUNCTION(BlueprintCallable, Category = "Pausing")
		void Resume();

	APlayerCameraManager* CameraManager;
	AController* Controller;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pausing")
		bool bMainMenu;

	void RestartLevel();

	/***********************************************************************************************************************************************
	* Atom and Molecule Public Functions and Data
	***********************************************************************************************************************************************/
	UFUNCTION(BlueprintCallable, Category = "Spawning")
		FParticleContainer_ SpawnAtom(FAtom Atom, FVector Location);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
		void SpawnNucleus();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
		void MakeMolecule(ASubAtomic_Particle* Particle);

	// Called to clear current atom and set new one
	UFUNCTION(BlueprintCallable, Category = "Spawning")
		void SetNewAtom(FAtom Atom, bool bReset);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FAtom CurrentAtom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<FAtom> AllAtoms;

	// Called to clear current atom only
	void ResetAtom();

	//Valence Rings
	void SpawnValenceRing();
	TArray<AValenceRing*> SpawnValenceRingAt(FAtom Atom, FVector Location);
	void AdjustValence();
	void ChangeValenceSpeed(float NewSpeed);
	void ChangeValenceSpeed(float NewSpeed, FAtom Atom);

	/***********************************************************************************************************************************************
	* Scripted Events
	***********************************************************************************************************************************************/
	// Called after an audio clip stops playing, used in this project for scripting events
	UFUNCTION()
		void OnAudioFinished();

	UFUNCTION(BlueprintCallable, Category = "Scripting")
		void StartScript();

	UFUNCTION(BlueprintCallable, Category = "Scripting")
		void StartChallenge();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scripting")
		bool Tutorial;

	void TutorialMode(AActor* ActorHit);
	void ChallengeMode(AActor* ActorHit);

	void GameStateIncrement();
	void ChallengeIncrement();
	void TriggerErrorSound();

	FTimerHandle GamestateTimer;
	FTimerHandle DeleteTimer;
	FTimerHandle ValenceTimer;

	/***********************************************************************************************************************************************
	* Audio
	***********************************************************************************************************************************************/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		UAudioComponent* AudioPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		UAudioComponent* BackgroundMusic;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Component")
		UAudioComponent* ErrorPlayer;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		TArray<USoundBase*> TutorialAudioClips;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		TArray<USoundBase*> ChallengeAudioClips;

	/***********************************************************************************************************************************************
	* Atoms and Molecules Data
	***********************************************************************************************************************************************/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
		FVector TargetPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FMoleculeStruct CurrentMolecule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<FMoleculeStruct> AllMolecules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FRotator ElectronsAngle;

	TArray<FAtom> CurrentAtoms;
	
	int32 CurrentProtons;
	int32 CurrentNeutrons;
	int32 CurrentElectrons;
	TArray<AActor*> ParticlesInScene;
	int32 LastFullAtomTag;

	/***********************************************************************************************************************************************
	* Stored Blueprints
	***********************************************************************************************************************************************/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprints")
		TSubclassOf<AProton> ProtonBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprints")
		TSubclassOf<ANeutron> NeutronBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprints")
		TSubclassOf<AElectron> ElectronBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprints")
		TArray<TSubclassOf<AMolecule>> MoleculeBPs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprints")
		TSubclassOf<AActor> ValenceRingBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprints")
		TSubclassOf<AActor> StaticValenceRingBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blueprints")
		TSubclassOf<AChallengeButton> ButtonBP;

	/***********************************************************************************************************************************************
	* Scripted Events
	***********************************************************************************************************************************************/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool StartRotating;

	void StartRotationScript();
	void StartMovingScript();
	bool StartMoving;

	TArray<UStaticMeshComponent*> LabParts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scripting")
	bool LabVisible;
	bool CanSelect;
	int32 GameState;
	int32 ChallengeState;

	/***********************************************************************************************************************************************
	* Spawning Scripted actors
	***********************************************************************************************************************************************/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		TArray<UClass*> ActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		TArray<float> ActorLifespans;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		TArray<FVector> LocationToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		TArray<FRotator> RotationToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		TArray<ASpawnVolume*> ParticleSpawns;

	FTimerHandle SpawnTimer;
	FTimerHandle SpawnTimer2;
	int32 LastActorSpawned;
	AActor* CurrentElementSymbol;

	void SpawnScriptedActor();
	//void DeleteUnUsedParticles();
	void SpawnElementSymbol();
	void DeleteElementSymbol();
	void SpawnTutorialEndSequence();
	void EndSequenceHelper_ATOMS();
	void EndSequenceHelper_Molecules();
	void EndSequenceHelper_Earth();

	/***********************************************************************************************************************************************
	* Buttons
	***********************************************************************************************************************************************/
	TArray<AChallengeButton*> ButtonsInScene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons")
		TArray<FVector> ButtonSpawns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons")
		TArray<UMaterialInterface*> ButtonMats;

	void ClickButton(AActor* ActorHit);
	void InitializeButtons();
	void DisableButtons();
	void EnableButtons();
	void EnableConfirmButton();

	bool ButtonsEnabled;

	/***********************************************************************************************************************************************
	* Pausing
	***********************************************************************************************************************************************/
	void Pause();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pausing")
		FVector PauseSphere;

	FVector BeforePauseLoc;
	FRotator BeforePauseRot;
};