// Fill out your copyright notice in the Description page of Project Settings.

#include "BP_SimulateFMU.h"
#include <string>
#include "FMU2.h"
//#include "fmi2Functions.h"

float UBP_SimulateFMU::SimulateFMU(const float In) {
	UE_LOG(LogTemp, Warning, TEXT("DemoText"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));

	const std::string guid = "{1d19fee2-02f1-4ae7-b863-b6f380f15015}";
	const std::string modelIdentifier = "test";
	FString fullPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() + "../test");
	const std::string unzipDir = std::string(TCHAR_TO_UTF8(*fullPath));
	const std::string instanceName = "instance";


	FString DebugLog(unzipDir.c_str());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *DebugLog);

	fmikit::FMU2Slave fmu = fmikit::FMU2Slave(guid, modelIdentifier, unzipDir, instanceName);

	const fmi2Real startTime = 0.;
	const fmi2Real stopTime = 1.;
	const fmi2Real stepSize = 0.1;
	const fmi2Real tolerance = 0.0001;

	fmu.instantiate(true);
	UE_LOG(LogTemp, Warning, TEXT("instantiate complete!"));

	fmu.setupExperiment(true, tolerance, startTime, true, stopTime);
	UE_LOG(LogTemp, Warning, TEXT("setupExperiment complete!"));

	fmu.enterInitializationMode();
	UE_LOG(LogTemp, Warning, TEXT("enterInitializationMode complete!"));

	fmu.exitInitializationMode();
	UE_LOG(LogTemp, Warning, TEXT("exitInitializationMode complete!"));

	fmi2Real time = 0;
	double value;
	//FString valueString;

	while (time <= stopTime) {
		value = fmu.getReal(33554432);
		UE_LOG(LogTemp, Warning, TEXT("%s, %s"), *FString::SanitizeFloat(time), *FString::SanitizeFloat(value));
		fmu.doStep(stepSize);
		time += stepSize;
	}
	UE_LOG(LogTemp, Warning, TEXT("Simulation complete!"));
	//std::map<std::string, double> map;
	//std::vector<std::map> vectorOfMaps;

	fmi2Boolean bStatus = fmu.terminated();
	UE_LOG(LogTemp, Warning, TEXT("Simulation %s"), bStatus ? TEXT("succeeded!") : TEXT("failed!"));
	UE_LOG(LogTemp, Warning, TEXT("That previous message is a test... need to check the terminate() function"));

	//outputMap={"key":value}

	//	outputMap[0] -> value
	//	outputMap["key"]->value

	//results = SimulateEnd();

	//outputMap = {};
	//for i in result:
	//	outputMap[i] = string;
	//	outputMap[i][string] = value;

	return FMath::Pow(In, 2);
}