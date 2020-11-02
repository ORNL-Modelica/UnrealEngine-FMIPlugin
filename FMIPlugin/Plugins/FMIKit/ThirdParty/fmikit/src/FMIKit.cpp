// Copyright Epic Games, Inc. All Rights Reserved.

#include "FMIKit.h"
#include "Misc/LocalTimestampDirectoryVisitor.h"

#define LOCTEXT_NAMESPACE "FFMIKitModule"

void FFMIKitModule::StartupModule()
{

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	//Search project plugins folder for Dll
	FString dllName = "zip.dll";
	FString TPLPath = FPaths::ProjectDir() + "Plugins/FMIKit/ThirdParty";
	if (SearchForDllPath(TPLPath, dllName))
	{
	}
	else
	{
		//Stop loading - plugin required DLL to load successfully
		checkf(false, TEXT("Failed to load dll"));
	}
	dllName = "zlib.dll";
	if (SearchForDllPath(TPLPath, dllName))
	{
	}
	else
	{
		//Stop loading - plugin required DLL to load successfully
		checkf(false, TEXT("Failed to load dll"));
	}
}

void FFMIKitModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FPlatformProcess::FreeDllHandle(DLLHandle);
}

bool FFMIKitModule::SearchForDllPath(FString _searchBase, FString _dllName)
{
	//Search Plugins folder for an instance of Dll.dll, and add to platform search path
	TArray<FString> directoriesToSkip;
	IPlatformFile &PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FLocalTimestampDirectoryVisitor Visitor(PlatformFile, directoriesToSkip, directoriesToSkip, false);
	PlatformFile.IterateDirectory(*_searchBase, Visitor);

	for (TMap<FString, FDateTime>::TIterator TimestampIt(Visitor.FileTimes); TimestampIt; ++TimestampIt)
	{
		const FString file = TimestampIt.Key();
		const FString filePath = FPaths::GetPath(file);
		const FString fileName = FPaths::GetCleanFilename(file);
		if (fileName.Compare(_dllName) == 0)
		{
			FPlatformProcess::AddDllDirectory(*filePath); // only load dll when needed for use. Broken with 4.11.
			switch (handle) {
			case 0:
				DLLHandle = FPlatformProcess::GetDllHandle(*file);
				break;
			case 1:
				DLLHandle2 = FPlatformProcess::GetDllHandle(*file);
				break;
			default:
				//DLLHandle3 = FPlatformProcess::GetDllHandle(*file);
				break;
			}
			handle++;
			return true;
		}
	}
	return false;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFMIKitModule, FMIKit)