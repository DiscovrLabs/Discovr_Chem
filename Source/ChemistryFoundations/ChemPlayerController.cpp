// Fill out your copyright notice in the Description page of Project Settings.

#include "ChemistryFoundations.h"
#include "ChemPlayerController.h"


AChemPlayerController::AChemPlayerController()
{
	bShouldPerformFullTickWhenPaused = true;
	SetTickableWhenPaused(true);
}