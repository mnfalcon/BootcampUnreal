// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealBootcamp3GameMode.h"
#include "UnrealBootcamp3Character.h"
#include "UObject/ConstructorHelpers.h"

AUnrealBootcamp3GameMode::AUnrealBootcamp3GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
