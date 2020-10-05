// Fill out your copyright notice in the Description page of Project Settings.


#include "FmuActorComponent.h"
#include "unzipper.hpp"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UFmuActorComponent::UFmuActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFmuActorComponent::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("UFmuActorComponent::BeginPlay()"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("UFmuActorComponent::BeginPlay()"));
	Super::BeginPlay();

	mStartLocation = GetOwner()->GetActorLocation();
	mNewLocation = mStartLocation;

	UE_LOG(LogTemp, Warning, TEXT("DemoText"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));

	// Extract from an .fmu file
	{
		FString fullPath = FPaths::ConvertRelativePathToFull(mPath.FilePath);
		std::string sPath = TCHAR_TO_UTF8(*fullPath);
		size_t lastindex = sPath.find_last_of(".");
		mUnzipDir = sPath.substr(0, lastindex);
		unzip(sPath, mUnzipDir);
	}

	// These should be populated from the extracted ModelDescription.xml
	{
		mGuid = "{1d19fee2-02f1-4ae7-b863-b6f380f15015}";
		mModelIdentifier = "test";
		mInstanceName = "instance";
		mStartTime = 0.;
		mStopTime = 1.* StopTimeMultiplier;
		mStepSize = 0.1;
		mTolerance = 0.0001;
		mTimeLast = mStartTime;
		mTimeNow = mStartTime;
	}

	FString DebugLog(mUnzipDir.c_str());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *DebugLog);

	mFmu = new fmikit::FMU2Slave(mGuid, mModelIdentifier, mUnzipDir, mInstanceName);

	mFmu->instantiate(true);
	UE_LOG(LogTemp, Warning, TEXT("instantiate complete!"));

	mFmu->setupExperiment(true, mTolerance, mStartTime, true, mStopTime);
	UE_LOG(LogTemp, Warning, TEXT("setupExperiment complete!"));

	mFmu->enterInitializationMode();
	UE_LOG(LogTemp, Warning, TEXT("enterInitializationMode complete!"));

	mFmu->exitInitializationMode();
	UE_LOG(LogTemp, Warning, TEXT("exitInitializationMode complete!"));
	
	mLoaded = true;
}


// Called every frame
void UFmuActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!mLoaded)
		return;

	mTimeNow += DeltaTime;
	if (!(mTimeNow > mTimeLast + mStepSize / SpeedMultiplier))
		return;

	if (mTimeLast >= mStopTime / SpeedMultiplier)
		return;

    mTimeLast += mStepSize / SpeedMultiplier;
	mFmu->doStep(mStepSize);
	float x = mFmu->getReal(33554432) * DistanceMultiplier[0] + mStartLocation[0];
	float y = mFmu->getReal(33554433) * DistanceMultiplier[1] + mStartLocation[1];
	float z = mFmu->getReal(33554434) * DistanceMultiplier[2] + mStartLocation[2];
	mNewLocation = {x , y , z};
	GetOwner()->SetActorLocation(mNewLocation);
}

