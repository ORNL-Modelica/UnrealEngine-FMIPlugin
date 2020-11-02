// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FFMIKitModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	bool SearchForDllPath(FString _searchBase, FString _dllName);

	int handle = 0;
	void *DLLHandle;
	void *DLLHandle2;
};
