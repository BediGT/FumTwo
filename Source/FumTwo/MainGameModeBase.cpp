// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameModeBase.h"
#include "Player/PlayerCharacter.h"

FUMTWO_API AMainGameModeBase::AMainGameModeBase()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
}