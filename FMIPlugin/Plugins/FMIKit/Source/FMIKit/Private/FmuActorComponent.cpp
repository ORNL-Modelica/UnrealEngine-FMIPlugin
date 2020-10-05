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

	StartLocation = GetOwner()->GetActorLocation();
	NewLocation = StartLocation;

	UE_LOG(LogTemp, Warning, TEXT("DemoText"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));

	// We want to extract from an .fmu file
	FString fullPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() + mPath);
	mUnzipDir = std::string(TCHAR_TO_UTF8(*fullPath));

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
		//float value = mFmu->getReal(33554432);
	NewLocation = {(float) mFmu->getReal(33554432)* DistanceMultiplier[0] + StartLocation[0], (float) mFmu->getReal(33554433)* DistanceMultiplier[1] + StartLocation[1], (float) mFmu->getReal(33554434)* DistanceMultiplier[2] + StartLocation[2] };
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(mTimeLast) + " " + FString::SanitizeFloat(value));
	//UE_LOG(LogTemp, Warning, TEXT("%s, %s"), *FString::SanitizeFloat(mTimeLast), *FString::SanitizeFloat(value));

	GetOwner()->SetActorLocation(NewLocation);
}

