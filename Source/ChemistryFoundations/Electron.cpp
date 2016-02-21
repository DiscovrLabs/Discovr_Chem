// Fill out your copyright notice in the Description page of Project Settings.

#include "ChemistryFoundations.h"
#include "ChemPlayer.h"
#include "Electron.h"

AElectron::AElectron()
{
	Following = false;
}

void AElectron::BeginPlay()
{
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	Spawned = false;
	ShouldMoveVector = false;
	SpawnPoint = GetActorLocation();
}

// Called every frame
void AElectron::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->ShouldMove)
	{
		Target = Node->GetComponentLocation();
		FVector tempTarget = FMath::VInterpTo(this->GetActorLocation(), Target, DeltaTime, 2);
		this->SetActorLocation(tempTarget);

		if ((this->GetActorLocation() - Target).Size() < 1)
		{
			AChemPlayer* TempPlayer = Cast<AChemPlayer>(Player);
			//UE_LOG(LogTemp, Log, TEXT("incoming electrons is %d"), TempPlayer->CurrentAtom.IncomingElectrons);
			(Spawned) ? MyAtom->IncomingElectrons-- : TempPlayer->CurrentAtom.IncomingElectrons--;
			if (MyAtom != NULL)
			{
				TempPlayer->ChangeValenceSpeed(180, *MyAtom);
			}
			else
			{
				TempPlayer->ChangeValenceSpeed(180);
			}
			this->ShouldMove = false;
			this->Following = true;
		}
	}
	else if (this->ShouldMoveVector)
	{
		FVector tempTarget = FMath::VInterpTo(this->GetActorLocation(), Target, DeltaTime, 2);
		this->SetActorLocation(tempTarget);

		if ((this->GetActorLocation() - Target).Size() < 1)
		{
			AChemPlayer* TempPlayer = Cast<AChemPlayer>(Player);
			(Spawned) ? MyAtom->IncomingElectrons-- : TempPlayer->CurrentAtom.IncomingElectrons--;
			if (MyAtom->ProtonNum != 0)
			{
				TempPlayer->ChangeValenceSpeed(180, *MyAtom);
			}
			else
			{
				TempPlayer->ChangeValenceSpeed(180);
			}
			this->ShouldMove = false;
		}
	}
	else if (ResetPos)
	{
		FVector tempTarget = FMath::VInterpTo(this->GetActorLocation(), Target, DeltaTime, 2);
		this->SetActorLocation(tempTarget);

		if ((this->GetActorLocation() - Target).Size() < 1)
		{
			this->ResetPos = false;
			Clicked = false;
		}
	}
	else if (this->Following && Node != NULL)
		this->SetActorLocation(Node->GetComponentLocation());
}

void AElectron::MoveToNode()
{
	ShouldMove = true;
	Following = false;
	Clicked = true;
}

void AElectron::SetNode(USceneComponent* NewNode)
{
	Node = NewNode;
}

void AElectron::Reset()
{
	ShouldMove = ShouldMoveVector = false;
	Following = false;
	Node = NULL;
	Target = SpawnPoint;
	ResetPos = true;
}

void AElectron::Move(FVector TargetObject, FVector TargetOffset, FRotator TargetRot)
{
	ShouldMove = false;
	Following = false;
	ShouldMoveVector = true;
	Target = TargetObject;
}