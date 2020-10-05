// Fill out your copyright notice in the Description page of Project Settings.


#include "A_FMU.h"

#include "XmlFile.h"
#include "unzipper.hpp"

// Sets default values
AA_FMU::AA_FMU()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

//// Called when actor is created or any updates are made to it
void AA_FMU::OnConstruction(const FTransform& Transform)
{
	UE_LOG(LogTemp, Warning, TEXT("test"));

	// Extract FMU
	//if (mPath.FilePath.IsEmpty())
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Path to .fmu is empty."));
	//	return;
	//}
	FString fullPath = FPaths::ConvertRelativePathToFull(mPath.FilePath);
	//std::string sPath = TCHAR_TO_UTF8(*fullPath);
	//size_t lastindex = sPath.find_last_of(".");
	//mUnzipDir = sPath.substr(0, lastindex);
	//unzip(sPath, mUnzipDir);
	
	// Parse XML
}

// Called when the game starts or when spawned
void AA_FMU::BeginPlay()
{
	Super::BeginPlay();
	

	UE_LOG(LogTemp, Error, TEXT("%s"), *mPath.FilePath);
	//// We want to extract from an .fmu file
	//FString fullPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() + Path);
	//UnzipDir = std::string(TCHAR_TO_UTF8(*fullPath));

	//// These should be populated from the extracted ModelDescription.xml
	//{
	//	Guid = "{1d19fee2-02f1-4ae7-b863-b6f380f15015}";
	//	ModelIdentifier = "test";
	//	InstanceName = "instance";
	//	StartTime = 0.;
	//	StopTime = 1. * StopTimeMultiplier;
	//	StepSize = 0.1;
	//	Tolerance = 0.0001;
	//	TimeLast = mStartTime;
	//	TimeNow = mStartTime;
	//}

	//FString DebugLog(UnzipDir.c_str());
	//Fmu = new fmikit::FMU2Slave(Guid, ModelIdentifier, UnzipDir, InstanceName);
	//Fmu->instantiate(true);
	//Fmu->setupExperiment(true, Tolerance, StartTime, true, StopTime);
	//Fmu->enterInitializationMode();
	//Fmu->exitInitializationMode();
	//Loaded = true;

}

// Called every frame
void AA_FMU::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

