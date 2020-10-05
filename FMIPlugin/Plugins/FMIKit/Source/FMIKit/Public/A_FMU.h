// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include "FMU2.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A_FMU.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FMIKIT_API AA_FMU : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AA_FMU();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//// Called when actor is created or any updates are made to it
	//virtual void OnConstruction() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ExtractFMU();
	void ParseXML();
	
	std::string mUnzipDir;
	std::string mFMIVersion;
	std::string mModelIdentifier;
	std::string mGuid;
	std::map<FString, int> mValRefMap;
	bool mLoaded = false;

	//User controls
	// float StartTime = 0.f; //update to default fmu xml
	// float StopTime = 1.f; // update to default fmu xml
	// bool bRestart = false;
	// bool bStart = false;
	// bool bStop = false;
	// bool bPause = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FFilePath mPath = { FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() , "../test.fmu") };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float SpeedMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FVector DistanceMultiplier = { 1.f,1.f,1.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float StopTimeMultiplier = 1.f;


	//fmikit::FMU2Slave* mFmu = nullptr;


	//std::string mInstanceName;
	fmi2Real mStartTime;
	fmi2Real mStopTime;
	//fmi2Real mStepSize;
	fmi2Real mTolerance;
	//fmi2Real mTimeLast;
	//fmi2Real mTimeNow;
};
