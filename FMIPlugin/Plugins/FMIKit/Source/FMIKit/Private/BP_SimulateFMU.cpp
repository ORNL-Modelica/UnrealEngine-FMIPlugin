// Fill out your copyright notice in the Description page of Project Settings.

#include "BP_SimulateFMU.h"

#include "FMU2.h"

float UBP_SimulateFMU::SimulateFMU(const float In) {
	UE_LOG(LogTemp, Warning, TEXT("DemoText"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));

	const std::string guid = "{1d19fee2-02f1-4ae7-b863-b6f380f15015}";
	const std::string modelIdentifier = "test";
	FString fullPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() + "../test");
	const std::string unzipDirectory = std::string(TCHAR_TO_UTF8(*fullPath));
	const std::string instanceName = "instance";
	fmikit::FMU2 *fmu = new fmikit::FMU2(guid, modelIdentifier, unzipDirectory, instanceName, nullptr, nullptr);
	fmu->instantiate(true);
	fmu->setupExperiment(true, .0001, 0., true, 1.);

	return FMath::Pow(In, 2);
}