// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RolliePollie.h"
#include "RolliePollieGameMode.h"
#include "RolliePollieBall.h"

ARolliePollieGameMode::ARolliePollieGameMode()
{
	// set default pawn class to our ball
	DefaultPawnClass = ARolliePollieBall::StaticClass();
}
