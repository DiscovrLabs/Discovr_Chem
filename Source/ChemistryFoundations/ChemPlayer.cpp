// Fill out your copyright notice in the Description page of Project Settings.

#include "ChemistryFoundations.h"
#include "SubAtomic_Particle.h"
#include "Proton.h"
#include "Neutron.h"
#include "Electron.h"
#include "ChemPlayer.h"


// Sets default values
AChemPlayer::AChemPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetTickableWhenPaused(true);

	AudioPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("Narrator"));
	AudioPlayer->OnAudioFinished.AddDynamic(this, &AChemPlayer::OnAudioFinished);
	AudioPlayer->SetTickableWhenPaused(false);
	BackgroundMusic = CreateDefaultSubobject<UAudioComponent>(TEXT("BackgroundMusic"));
	BackgroundMusic->SetTickableWhenPaused(true);
	BackgroundMusic->SetUISound(true);

	ErrorPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("ErrorSound"));
	ErrorPlayer->SetTickableWhenPaused(false);

	GameState = 0;
	ChallengeState = 0;
	CanSelect = false;
	StartRotating = false;
	StartMoving = false;
	Tutorial = true;
	bMainMenu = true;

	// Set TargetPoint
	TargetPoint = FVector(280.f, 0, 80.f);

	CurrentProtons = 0;
	CurrentNeutrons = 0;
	CurrentElectrons = 0;
	LastFullAtomTag = 0;
	LabVisible = false;
	LastActorSpawned = 0;
	ButtonsEnabled = false;
}

void AChemPlayer::RestartLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetMapName()));
}

void AChemPlayer::Pause()
{
	if (bMainMenu == false)
	{
		if (!UGameplayStatics::IsGamePaused(GetWorld()))
		{
			if (GetWorldTimerManager().IsTimerActive(SpawnTimer))
				GetWorldTimerManager().PauseTimer(SpawnTimer);

			if (GetWorldTimerManager().IsTimerActive(SpawnTimer2))
				GetWorldTimerManager().PauseTimer(SpawnTimer2);

			if (GetWorldTimerManager().IsTimerActive(GamestateTimer))
				GetWorldTimerManager().PauseTimer(GamestateTimer);

			if (GetWorldTimerManager().IsTimerActive(ValenceTimer))
				GetWorldTimerManager().PauseTimer(ValenceTimer);

			UGameplayStatics::SetGamePaused(GetWorld(), true);

			BeforePauseLoc = GetActorLocation();
			BeforePauseRot = Controller->GetControlRotation();
			SetActorLocation(PauseSphere);
			Controller->SetControlRotation(FRotator::ZeroRotator);
		}
		//else
			//Resume();
	}
}

void AChemPlayer::Resume()
{
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		if (GetWorldTimerManager().IsTimerPaused(SpawnTimer))
			GetWorldTimerManager().UnPauseTimer(SpawnTimer);

		if (GetWorldTimerManager().IsTimerPaused(SpawnTimer2))
			GetWorldTimerManager().UnPauseTimer(SpawnTimer2);

		if (GetWorldTimerManager().IsTimerPaused(GamestateTimer))
			GetWorldTimerManager().UnPauseTimer(GamestateTimer);

		if (GetWorldTimerManager().IsTimerPaused(ValenceTimer))
			GetWorldTimerManager().UnPauseTimer(ValenceTimer);

		UGameplayStatics::SetGamePaused(GetWorld(), false);

		SetActorLocation(BeforePauseLoc);
		Controller->SetControlRotation(BeforePauseRot);
	}
}

// Called when the game starts or when spawned
void AChemPlayer::BeginPlay()
{
	Super::BeginPlay();

	UWorld* MyWorld = GetWorld();

	Controller = GetController();
	CameraManager = UGameplayStatics::GetPlayerCameraManager(MyWorld, 0);
	ErrorPlayer->SetSound(TutorialAudioClips[TutorialAudioClips.Num() - 1]);

	for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->ActorHasTag(TEXT("Lab")))
			LabParts.Add(ActorItr->GetStaticMeshComponent());
	}

	CurrentAtom = AllAtoms[0];

	TArray<AActor*> TempActors;
	UGameplayStatics::GetAllActorsOfClass(MyWorld, APAL::StaticClass(), TempActors);
	for (AActor* TempActor : TempActors)
	{
		PALs.Add(Cast<APAL>(TempActor));
	}
}

// Called every frame
void AChemPlayer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime ); 
	if (Tutorial)
	{
		if (StartRotating && !UGameplayStatics::IsGamePaused(GetWorld()))
		{
			FRotator TempTarget = FMath::RInterpConstantTo(Controller->GetControlRotation(), FRotator(0.f, 270.f, 0.f), DeltaTime, 15.3f);
			Controller->SetControlRotation(TempTarget);
			if (Controller->GetControlRotation().Equals(FRotator(0.f, 270.f, 0.f), 2.f))
			{
				StartRotating = false;
				GetWorldTimerManager().SetTimer(GamestateTimer, FTimerDelegate::CreateUObject(this, &AChemPlayer::StartMovingScript), 2.f, false);
			}
		}
		else if (StartMoving && !UGameplayStatics::IsGamePaused(GetWorld()))
		{
			FVector Target = FVector(-280.f, -2640.f, GetActorLocation().Z);
			FVector TempTarget = FMath::VInterpTo(this->GetActorLocation(), Target, DeltaTime, 0.35f);
			this->SetActorLocation(TempTarget);

			if (GetActorLocation().Y < -2375 && LabVisible == false)
			{
				for (UStaticMeshComponent* TempMesh : LabParts)
				{
					TempMesh->SetVisibility(true);
				}
				LabVisible = true;
				SpawnUsableParticles();
			}

			if ((this->GetActorLocation() - Target).Size() < 1)
			{
				StartMoving = false;
			}
		}
	}
}

// Called to bind functionality to input
void AChemPlayer::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AChemPlayer::Raycast);
	InputComponent->BindAction("Pause", IE_Pressed, this, &AChemPlayer::Pause);
}

void AChemPlayer::DisableActor(AActor* Actor, bool bDisable)
{
	if (bDisable)
	{
		Actor->SetActorHiddenInGame(true);
		Actor->SetActorEnableCollision(false);
		Actor->SetActorTickEnabled(false);
	}
	else
	{
		Actor->SetActorHiddenInGame(false);
		Actor->SetActorEnableCollision(true);
		Actor->SetActorTickEnabled(true);
	}
}

// Called to spawn an atom
FParticleContainer_ AChemPlayer::SpawnAtom(FAtom Atom, FVector Location)
{
	UWorld* ThisWorld = GetWorld();
	//Creating Tags
	TArray<FName> SceneTags;
	SceneTags.Add(FName(*FString::FromInt(LastFullAtomTag)));
	SceneTags.Add(FName(*FString::FromInt(Atom.ID)));

	CurrentAtoms.Add(Atom);
	FParticleContainer_ TempContainer;
	for (FVector Offset : CurrentAtoms[LastFullAtomTag].ProtonOffsets)
	{
		AProton* TempProton = ThisWorld->SpawnActor<AProton>(ProtonBP,Location + Offset, FRotator::ZeroRotator);
		TempProton->Tags = SceneTags;
		ParticlesInScene.Add(TempProton);
		TempContainer.Protons.Add(TempProton);
	}

	for (FVector Offset : CurrentAtoms[LastFullAtomTag].NeutronOffsets)
	{
		ANeutron* TempNeutron = ThisWorld->SpawnActor<ANeutron>(NeutronBP, Location + Offset, FRotator::ZeroRotator);
		TempNeutron->Tags = SceneTags;
		ParticlesInScene.Add(TempNeutron);
		TempContainer.Neutrons.Add(TempNeutron);
	}
	TempContainer.ValenceRings = CurrentAtoms[LastFullAtomTag].ValenceRings = SpawnValenceRingAt(CurrentAtoms[LastFullAtomTag], Location);
	ChangeValenceSpeed(0, CurrentAtoms[LastFullAtomTag]);

	for (int i = 0; i < CurrentAtoms[LastFullAtomTag].ElectronNum; i++)
	{
		AElectron* TempElectron = ThisWorld->SpawnActor<AElectron>(ElectronBP, Location, FRotator::ZeroRotator);
		TempElectron->Tags = SceneTags;
		TempElectron->SetNode(CurrentAtoms[LastFullAtomTag].ValenceRings[(i >= 2) ? 1 : 0]->Nodes[(i >= 2) ? i - 2 : i]);
		TempElectron->Spawned = true;
		CurrentAtoms[LastFullAtomTag].IncomingElectrons++;
		TempElectron->MoveToNode();
		TempElectron->MyAtom = &CurrentAtoms[LastFullAtomTag];
		ParticlesInScene.Add(TempElectron);
		TempContainer.Electrons.Add(TempElectron);
	}
	LastFullAtomTag++;
	return TempContainer;
}

void AChemPlayer::SpawnNucleus()
{
	UE_LOG(LogTemp, Log, TEXT("Nuculus"));
	UWorld* ThisWorld = GetWorld();

	for (FVector Offset : CurrentAtom.ProtonOffsets)
	{
		AProton* TempProton = ThisWorld->SpawnActor<AProton>(ProtonBP, TargetPoint + Offset, FRotator::ZeroRotator);
		ParticlesInScene.Add(TempProton);
	}
	CurrentProtons = CurrentAtom.ProtonNum;

	for (FVector Offset : CurrentAtom.NeutronOffsets)
	{
		ANeutron* TempNeutron = ThisWorld->SpawnActor<ANeutron>(NeutronBP, TargetPoint + Offset, FRotator::ZeroRotator);
		ParticlesInScene.Add(TempNeutron);
	}
	CurrentNeutrons = CurrentAtom.NeutronNum;

	CurrentAtom.ValenceRings = SpawnValenceRingAt(CurrentAtom, TargetPoint);
}

// Called to spawn a molecule
void AChemPlayer::MakeMolecule(ASubAtomic_Particle* Particle)
{
	FVector AtomLocation = Particle->GetActorLocation();
	TArray<FName> AtomTags = Particle->Tags;
	TArray<int32> ParticleIndices;

	// Storing particle indices
	int32 index = 0;
	for (AActor* SceneParticle : ParticlesInScene)
	{
		if (SceneParticle->Tags[0] == AtomTags[0])
		{
			ParticleIndices.Add(index);
			SceneParticle->Destroy();
		}
		index++;
	}

	// Deleting particles from scene
	for (int i = ParticleIndices.Num()-1; i > -1; i--)
	{
		ParticlesInScene.RemoveAt(ParticleIndices[i]);
	}
	ParticleIndices.Empty();

	FAtom TempAtom = AllAtoms[FCString::Atoi(*AtomTags[1].ToString())];

	if (MoleculeBPs[TempAtom.ID])
	{
		int AtomIndex = 0;
		for (int i = 0; i < CurrentMolecule.AtomIDs.Num(); i++)
		{
			if (CurrentMolecule.AtomIDs[i] == TempAtom.ID)
			{
				AtomIndex = i;
				break;
			}
		}
		AMolecule* TempMolecule = GetWorld()->SpawnActor<AMolecule>(MoleculeBPs[TempAtom.ID], AtomLocation, FRotator::ZeroRotator);
		TempMolecule->Move(TargetPoint, CurrentMolecule.AtomOffsets[AtomIndex]);
		CurrentMolecule.AtomOffsets.RemoveAt(AtomIndex);
		CurrentMolecule.AtomIDs.RemoveAt(AtomIndex);

		if (CurrentMolecule.AtomIDs.Num() == 0)
		{
			SpawnTutorialEndSequence();
			GameStateIncrement();
		}
	}
	LastFullAtomTag--;
}

// Called to clear current atom and set new one
void AChemPlayer::SetNewAtom(FAtom Atom, bool bReset = true)
{
	ResetAtom();
	CurrentAtom = Atom;
}

// Called to clear current atom
void AChemPlayer::ResetAtom()
{
	CurrentProtons = 0;
	CurrentNeutrons = 0;
	CurrentElectrons = 0;
	
	for (AActor* Particle : ParticlesInScene)
	{
		Particle->Destroy();
	}
	
	ParticlesInScene.Empty();
	CurrentAtom.ValenceRings.Empty();
}

void AChemPlayer::Raycast()
{
	if (CanSelect)
	{
		FVector Start = CameraManager->GetCameraLocation() + FVector(0.f, 0.f, 13.f);
		FVector End = CameraManager->GetActorForwardVector() * 1500 + Start;

		FHitResult OutHit;
		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName("PlayerTrace"), false, this);

		//GetWorld()->DebugDrawTraceTag = "PlayerTrace";
		GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility, TraceParams);

		UE_LOG(LogTemp, Log, TEXT("Raycasted"));

		if (OutHit.bBlockingHit)
		{
			AActor* ActorHit = OutHit.GetActor();

			if (ActorHit->IsA(ASubAtomic_Particle::StaticClass()))
			{
				if (Tutorial)
					TutorialMode(ActorHit);
				else
					ChallengeMode(ActorHit);
			}
			else if (ActorHit->IsA(AChallengeButton::StaticClass()))
				ClickButton(ActorHit);
		}
	}
}

void AChemPlayer::TutorialMode(AActor* ActorHit)
{
	if (GameState < 11)
	{
		// If hit a Proton
		if (ActorHit->IsA(AProton::StaticClass()))
		{
			UE_LOG(LogTemp, Log, TEXT("Hit Proton"));
			AProton* Proton = Cast<AProton>(ActorHit);
			if (!Proton->Clicked)
			{
				if (CurrentProtons < CurrentAtom.ProtonNum)
				{
					Proton->Move(TargetPoint, CurrentAtom.ProtonOffsets[CurrentProtons]);
					ParticlesInScene.Add(Proton);
					CurrentProtons++;

					if (CurrentProtons == CurrentAtom.ProtonNum)
					{
						GetWorldTimerManager().SetTimer(GamestateTimer, FTimerDelegate::CreateUObject(this, &AChemPlayer::GameStateIncrement), 1.5f, false);
						CanSelect = false;
						if (CurrentAtom.NeutronNum == 0)
							GetWorldTimerManager().SetTimer(ValenceTimer, FTimerDelegate::CreateUObject(this, &AChemPlayer::SpawnValenceRing), 1.5f, false);
					}
				}
				else
				{
					TriggerErrorSound();
				}
			}

		}
		// If hit a Neutron
		else if (ActorHit->IsA(ANeutron::StaticClass()) && CurrentProtons >= CurrentAtom.ProtonNum)
		{
			UE_LOG(LogTemp, Log, TEXT("Hit Neutron"));
			ANeutron* Neutron = Cast<ANeutron>(ActorHit);
			if (!Neutron->Clicked)
			{
				if (CurrentNeutrons < CurrentAtom.NeutronNum)
				{

					Neutron->Move(TargetPoint, CurrentAtom.NeutronOffsets[CurrentNeutrons]);
					ParticlesInScene.Add(Neutron);
					CurrentNeutrons++;

					if (CurrentNeutrons == CurrentAtom.NeutronNum)
					{
						GetWorldTimerManager().SetTimer(GamestateTimer, FTimerDelegate::CreateUObject(this, &AChemPlayer::GameStateIncrement), 1.5f, false);
						CanSelect = false;
						GetWorldTimerManager().SetTimer(ValenceTimer, FTimerDelegate::CreateUObject(this, &AChemPlayer::SpawnValenceRing), 1.5f, false);
					}
				}
				else
				{
					TriggerErrorSound();
				}
			}
		}
		// If hit an electron
		else if (ActorHit->IsA(AElectron::StaticClass()) && CurrentProtons >= CurrentAtom.ProtonNum && CurrentNeutrons >= CurrentAtom.NeutronNum)
		{
			UE_LOG(LogTemp, Log, TEXT("Hit Electron"));
			AElectron* Electron = Cast<AElectron>(ActorHit);
			if (!Electron->Clicked)
			{
				if (CurrentElectrons < CurrentAtom.ElectronNum)
				{
					Electron->SetNode(CurrentAtom.ValenceRings[(CurrentElectrons >= 2) ? 1 : 0]->Nodes[(CurrentElectrons >= 2) ? CurrentElectrons - 2 : CurrentElectrons]);
					CurrentAtom.IncomingElectrons++;
					ChangeValenceSpeed(0);
					Electron->MoveToNode();
					ParticlesInScene.Add(Electron);
					CurrentElectrons++;

					if (CurrentElectrons == CurrentAtom.ElectronNum)
					{
						CanSelect = false;
						GetWorldTimerManager().SetTimer(GamestateTimer, FTimerDelegate::CreateUObject(this, &AChemPlayer::GameStateIncrement), 1.5f, false);
						if (GameState == 4)
							GetWorldTimerManager().SetTimer(SpawnTimer, FTimerDelegate::CreateUObject(this, &AChemPlayer::SpawnScriptedActor), 3.5f, false);
						else if (GameState == 8)
						{
							GetWorldTimerManager().SetTimer(SpawnTimer, FTimerDelegate::CreateUObject(this, &AChemPlayer::SpawnScriptedActor), 3.5f, false);
							GetWorldTimerManager().SetTimer(SpawnTimer2, FTimerDelegate::CreateUObject(this, &AChemPlayer::SpawnScriptedActor), 8.5f, false);
						}
					}
				}
			}
		}
		else
		{
			TriggerErrorSound();
		}
	}
	else if (GameState == 11)
	{
		MakeMolecule((ASubAtomic_Particle*)ActorHit);
	}
}

void AChemPlayer::ChallengeMode(AActor* ActorHit)
{
	// If hit an electron
	if (ActorHit->IsA(AElectron::StaticClass()) && CurrentProtons >= CurrentAtom.ProtonNum && CurrentNeutrons >= CurrentAtom.NeutronNum && !ButtonsEnabled)
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Electron"));
		if ((CurrentAtom.ElectronNum > 2 && CurrentElectrons < 10) || (CurrentAtom.ElectronNum <= 2 && CurrentElectrons < 2))
		{
			AElectron* Electron = Cast<AElectron>(ActorHit);
			if (!Electron->Clicked)
			{
				Electron->SetNode(CurrentAtom.ValenceRings[(CurrentElectrons >= 2) ? 1 : 0]->Nodes[(CurrentElectrons >= 2) ? CurrentElectrons - 2 : CurrentElectrons]);
				CurrentAtom.IncomingElectrons++;
				UE_LOG(LogTemp, Log, TEXT("incoming electrons is %d"), CurrentAtom.IncomingElectrons);
				ChangeValenceSpeed(0);
				Electron->MoveToNode();
				ParticlesInScene.Add(Electron);
				CurrentElectrons++;
			}
		}
		else
		{
			TriggerErrorSound();
		}
	}
}

void AChemPlayer::InitializeButtons()
{
	UWorld* ThisWorld = GetWorld();
	for (int i = 0; i < ButtonSpawns.Num(); i++)
	{
		AChallengeButton* TempButton = ThisWorld->SpawnActor<AChallengeButton>(ButtonBP,ButtonSpawns[i], FRotator(0, 0, 90.f));
		if (TempButton)
		{
			TempButton->Num = i;
			TempButton->ButtonMesh->SetMaterial(0, ButtonMats[i]);
			DisableActor(TempButton, true);
			ButtonsInScene.Add(TempButton);
		}
	}
	ButtonsEnabled = false;
	//DisableActor(ButtonsInScene[ButtonsInScene.Num() - 1], false);
}

void AChemPlayer::ClickButton(AActor* ActorHit)
{
	if (CanSelect)
	{
		AChallengeButton* ClickedButton = Cast<AChallengeButton>(ActorHit);

		switch (ClickedButton->Num)
		{
		case 0:
			if (CurrentAtom.IncomingElectrons == 0)
			{
				SetNewAtom(AllAtoms[ChallengeState + 1]);
				CanSelect = false;
				ChallengeIncrement();
				DisableButtons();
			}
			break;
		case 1:
			if (CurrentAtom.ValenceRings[0]->RotatingMovementComponent->RotationRate.Yaw == SLOW)
				ChangeValenceSpeed(NORMAL);
			else
				ChangeValenceSpeed(FAST);
			break;
		case 2:
			if (CurrentAtom.ValenceRings[0]->RotatingMovementComponent->RotationRate.Yaw == FAST)
				ChangeValenceSpeed(NORMAL);
			else
				ChangeValenceSpeed(SLOW);
			break;
		case 3:
			if (CurrentAtom.IncomingElectrons == 0)
			{
				if (CurrentElectrons == CurrentAtom.ElectronNum)
				{
					AdjustValence();
					EnableButtons();
				}
				else
				{
					int32 tempInt = 0;
					for (int i = 0; i < ParticlesInScene.Num(); i++)
					{
						if (ParticlesInScene[i]->IsA(AElectron::StaticClass()))
						{
							if (tempInt == 0)
								tempInt = i;

							AElectron* tempElec = Cast<AElectron>(ParticlesInScene[i]);
							tempElec->Reset();
						}
					}
					for (int i = ParticlesInScene.Num() - 1; i >= tempInt; i--)
					{
						ParticlesInScene.RemoveAt(i);
					}
					CurrentElectrons = 0;
					TriggerErrorSound();
				}
			}
			break;
		}
	}
}

void AChemPlayer::DisableButtons()
{
	for (int i = 0; i < ButtonsInScene.Num() - 1; i++)
	{
		DisableActor(ButtonsInScene[i], true);
	}
	ButtonsEnabled = false;
	DeleteElementSymbol();
	GetWorldTimerManager().SetTimer(GamestateTimer, FTimerDelegate::CreateUObject(this, &AChemPlayer::EnableConfirmButton), 1.5f, false);
}

void AChemPlayer::EnableConfirmButton()
{
	DisableActor(ButtonsInScene[ButtonsInScene.Num() - 1], false);
	CanSelect = true;
}

void AChemPlayer::EnableButtons()
{
	for (int i = 0; i < ButtonsInScene.Num() - 1; i++)
	{
		DisableActor(ButtonsInScene[i], false);
	}
	ButtonsEnabled = true;
	DisableActor(ButtonsInScene[ButtonsInScene.Num() - 1], true);
}

void AChemPlayer::OnAudioFinished()
{
	if (Tutorial)
	{
		switch (GameState)
		{
		case 1:
			GameStateIncrement();
			GetWorldTimerManager().SetTimer(GamestateTimer, FTimerDelegate::CreateUObject(this, &AChemPlayer::GameStateIncrement), AudioPlayer->Sound->Duration + 1.f, false);
			break;
		case 3:
			ParticlesInScene.Empty();
			CurrentAtom = AllAtoms[0];
			CanSelect = true;
			break;
		case 4:
			CanSelect = true;
			break;
		case 5:
			GameStateIncrement();
			//Set CurrentAtom to Lithium
			SetNewAtom(AllAtoms[1]);
			break;
		case 6:
			CanSelect = true;
			break;
		case 7:
			CanSelect = true;
			break;
		case 8:
			CanSelect = true;
			break;
		case 9:
			GameStateIncrement();
			GetWorldTimerManager().SetTimer(GamestateTimer, FTimerDelegate::CreateUObject(this, &AChemPlayer::ResetAtom), 24.f, false);
			GetWorldTimerManager().SetTimer(SpawnTimer, FTimerDelegate::CreateUObject(this, &AChemPlayer::SpawnScriptedActor), 29.f, false);
			break;
		case 10:
			TargetPoint = FVector(-272, -2864, 1170);
			SpawnAtom(AllAtoms[0], FVector(-362.f, -3091.f, 1106.f));
			SpawnAtom(AllAtoms[0], FVector(-362.f, -3091.f, 1200.f));
			SpawnAtom(AllAtoms[2], FVector(-154.f, -3091.f, 1179.f));
			CurrentMolecule = AllMolecules[0];
			GameStateIncrement();
			break;
		case 11:
			CanSelect = true;
			break;
		case 12:
			GetWorldTimerManager().SetTimer(GamestateTimer, FTimerDelegate::CreateUObject(this, &AChemPlayer::RestartLevel), 2.f, false);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (ChallengeState)
		{
		case 1:
			ChallengeIncrement();
			InitializeButtons();
			SpawnElementSymbol();
			break;
		case 2:
			CanSelect = true;
			break;
		case 3:
			SpawnNucleus();
			DisableButtons();
			SpawnElementSymbol();
			break;
		case 4:
			SpawnNucleus();
			DisableButtons();
			SpawnElementSymbol();
			break;
		case 5:
			SpawnNucleus();
			DisableButtons();
			SpawnElementSymbol();
			break;
		case 6:
			SpawnNucleus();
			DisableButtons();
			SpawnElementSymbol();
			break;
		case 7:
			GetWorldTimerManager().SetTimer(GamestateTimer, FTimerDelegate::CreateUObject(this, &AChemPlayer::RestartLevel), 2.f, false);
			break;
		default:
			break;
		}
	}
}
/*
void AChemPlayer::DeleteUnUsedParticles()
{
	for (ASpawnVolume* Volume : ParticleSpawns)
	{
		Volume->DeleteParticles();
	}
}
*/
void AChemPlayer::StartScript()
{
	GameState = 1;
	AudioPlayer->SetSound(TutorialAudioClips[0]);
	AudioPlayer->Play(0.001f);
	GetWorldTimerManager().SetTimer(GamestateTimer, FTimerDelegate::CreateUObject(this, &AChemPlayer::StartRotationScript), 9.f, false);
}

void AChemPlayer::SpawnUsableParticles()
{
	for (ASpawnVolume* Volume : ParticleSpawns)
	{
		if (Volume != NULL && Volume->ActorHasTag("Spawn1"))
			Volume->SpawnParticles(0);
	}
}

void AChemPlayer::StartChallenge()
{
	TargetPoint = FVector(-5030, -1910, 1200);
	CurrentAtom = AllAtoms[0];
	AudioPlayer->SetSound(ChallengeAudioClips[0]);
	AudioPlayer->Play(0.001f);
	for (APAL* PAL : PALs)
	{
		PAL->UpdateAudioInfo(ChallengeAudioClips[0]);
	}
	StartRotationScript();
	SpawnNucleus();
	CurrentAtom.IncomingElectrons = 0;
	CanSelect = false;
	ChallengeState = 1;
	LastActorSpawned = 4;
	SetActorLocation(FVector(-5030, -1629, 1110));

	for (ASpawnVolume* Volume : ParticleSpawns)
	{
		if (Volume != NULL && Volume->ActorHasTag("Spawn2"))
			Volume->SpawnParticles(1);
	}
}

void AChemPlayer::StartRotationScript()
{
	StartRotating = true;
}

void AChemPlayer::StartMovingScript()
{
	StartMoving = true;
}

void AChemPlayer::GameStateIncrement()
{
	AudioPlayer->SetSound(TutorialAudioClips[GameState]);
	AudioPlayer->Play(0.001f);
	GameState++;
	for (APAL* PAL : PALs)
	{
		PAL->UpdateAudioInfo(TutorialAudioClips[GameState]);
	}
}

void AChemPlayer::ChallengeIncrement()
{
	AudioPlayer->SetSound(ChallengeAudioClips[ChallengeState]);
	AudioPlayer->Play(0.001f);
	ChallengeState++;
	for (APAL* PAL : PALs)
	{
		PAL->UpdateAudioInfo(ChallengeAudioClips[ChallengeState]);
	}
}

void AChemPlayer::TriggerErrorSound()
{
	ErrorPlayer->Play(0.001f);
}

void AChemPlayer::SpawnScriptedActor()
{
	if (ActorToSpawn[LastActorSpawned] != NULL)
	{
		AActor* Temp = GetWorld()->SpawnActor<AActor>(ActorToSpawn[LastActorSpawned], LocationToSpawn[LastActorSpawned], RotationToSpawn[LastActorSpawned]);
		Temp->SetLifeSpan(ActorLifespans[LastActorSpawned]);
	}

	LastActorSpawned++;
}

void AChemPlayer::SpawnElementSymbol()
{
	if (ActorToSpawn[LastActorSpawned] != NULL)
	{
		CurrentElementSymbol = GetWorld()->SpawnActor<AActor>(ActorToSpawn[LastActorSpawned], LocationToSpawn[4], RotationToSpawn[4]);
	}

	LastActorSpawned++;
}

void AChemPlayer::DeleteElementSymbol()
{
	if (CurrentElementSymbol != NULL)
		CurrentElementSymbol->Destroy();
}

void AChemPlayer::SpawnTutorialEndSequence()
{
	GetWorldTimerManager().SetTimer(SpawnTimer, FTimerDelegate::CreateUObject(this, &AChemPlayer::EndSequenceHelper_ATOMS), 6.f, false);
	GetWorldTimerManager().SetTimer(SpawnTimer2, FTimerDelegate::CreateUObject(this, &AChemPlayer::EndSequenceHelper_Molecules), 8.f, false);
	GetWorldTimerManager().SetTimer(SpawnTimer3, FTimerDelegate::CreateUObject(this, &AChemPlayer::EndSequenceHelper_Earth), 11.5f, false);
}

void AChemPlayer::EndSequenceHelper_ATOMS()
{
	AActor* Temp = GetWorld()->SpawnActor<AActor>(ActorToSpawn[9], LocationToSpawn[5], RotationToSpawn[5]);
	Temp->SetLifeSpan(ActorLifespans[4]);
}

void AChemPlayer::EndSequenceHelper_Molecules()
{
	AActor* Temp = GetWorld()->SpawnActor<AActor>(ActorToSpawn[11], LocationToSpawn[7], RotationToSpawn[7]);
	Temp->SetLifeSpan(ActorLifespans[4]);
}

void AChemPlayer::EndSequenceHelper_Earth()
{
	AActor* Temp = GetWorld()->SpawnActor<AActor>(ActorToSpawn[10], LocationToSpawn[6], RotationToSpawn[6]);
	Temp->SetLifeSpan(ActorLifespans[4]);
}

void AChemPlayer::SpawnValenceRing()
{
	UWorld* ThisWorld = GetWorld();
	FRotator RingRotation = FRotator(0, 0, 90);
	AValenceRing* TempRing = ThisWorld->SpawnActor<AValenceRing>(ValenceRingBP, TargetPoint, RingRotation);
	TempRing->SpawnNodes((CurrentAtom.ElectronNum > 2) ? 2 : CurrentAtom.ElectronNum);
	CurrentAtom.ValenceRings.Add(TempRing);
	ParticlesInScene.Add(TempRing);

	if (CurrentAtom.ElectronNum > 2)
	{
		TempRing = ThisWorld->SpawnActor<AValenceRing>(ValenceRingBP, TargetPoint, RingRotation);
		TempRing->SetActorScale3D(FVector(0.75f, 0.75f, 0.75f));
		TempRing->SpawnNodes(CurrentAtom.ElectronNum - 2);
		CurrentAtom.ValenceRings.Add(TempRing);
		ParticlesInScene.Add(TempRing);
	}
}

TArray<AValenceRing*> AChemPlayer::SpawnValenceRingAt(FAtom Atom, FVector Location)
{
	//Creating Tags
	TArray<FName> SceneTags;
	SceneTags.Add(FName(*FString::FromInt(LastFullAtomTag)));
	SceneTags.Add(FName(*FString::FromInt(Atom.ID)));

	UWorld* ThisWorld = GetWorld();
	FRotator RingRotation = FRotator(0, 0, 90);
	TArray<AValenceRing*> RingsArray;
	AValenceRing* TempRing = ThisWorld->SpawnActor<AValenceRing>(ValenceRingBP, Location, RingRotation);
	UE_LOG(LogTemp, Log, TEXT("Tutorial = %d"), Tutorial);

	if (Tutorial)
	{
		TempRing->SpawnNodes((Atom.ElectronNum > 2) ? 2 : Atom.ElectronNum);
	}
	else
	{
		TempRing->SpawnNodes(2);
	}
	TempRing->Tags = SceneTags;
	RingsArray.Add(TempRing);
	ParticlesInScene.Add(TempRing);

	if (Atom.ElectronNum > 2)
	{
		TempRing = ThisWorld->SpawnActor<AValenceRing>(ValenceRingBP, Location, RingRotation);
		TempRing->SetActorScale3D(FVector(0.75f, 0.75f, 0.75f));
		if (Tutorial)
		{
			TempRing->SpawnNodes(Atom.ElectronNum - 2);
		}
		else
		{
			TempRing->SpawnNodes(8);
		}
		TempRing->Tags = SceneTags;
		RingsArray.Add(TempRing);
		ParticlesInScene.Add(TempRing);
	}
	return RingsArray;
}

void AChemPlayer::AdjustValence()
{
	UWorld* ThisWorld = GetWorld();
	FRotator RingRotation = FRotator(0, 0, 90);

	if (CurrentAtom.ElectronNum > 2)
	{
		AValenceRing* TempRing = ThisWorld->SpawnActor<AValenceRing>(StaticValenceRingBP, TargetPoint, RingRotation);
		TempRing->SetActorScale3D(FVector(0.75f, 0.75f, 0.75f));
		TempRing->SpawnNodes(CurrentAtom.ElectronNum - 2);
		CurrentAtom.ValenceRings.Add(TempRing);
		ParticlesInScene.Add(TempRing);
		ChangeValenceSpeed(0);

		int32 TempInt = 0;
		for (int i = 0; i < ParticlesInScene.Num(); i++)
		{
			if (ParticlesInScene[i]->IsA(AElectron::StaticClass()))
			{
				if (TempInt > 1)
				{
					AElectron* tempElec = Cast<AElectron>(ParticlesInScene[i]);
					CurrentAtom.IncomingElectrons++;
					tempElec->SetNode(CurrentAtom.ValenceRings[2]->Nodes[TempInt - 2]);
					tempElec->MoveToNode();
				}
				TempInt++;
			}
		}
		DisableActor(CurrentAtom.ValenceRings[1], true);
	}
}

void AChemPlayer::ChangeValenceSpeed(float NewSpeed)
{
	ChangeValenceSpeed(NewSpeed, CurrentAtom);
}

void AChemPlayer::ChangeValenceSpeed(float NewSpeed, FAtom Atom)
{
	if (NewSpeed > 0)
	{
		if (Atom.IncomingElectrons == 0)
		{
			for (AValenceRing* tempRing : Atom.ValenceRings)
				tempRing->SetTargetRotationRate(NewSpeed);
		}

	}
	else
	{
		for (AValenceRing* tempRing : Atom.ValenceRings)
			tempRing->SetTargetRotationRate(NewSpeed);
	}
}
