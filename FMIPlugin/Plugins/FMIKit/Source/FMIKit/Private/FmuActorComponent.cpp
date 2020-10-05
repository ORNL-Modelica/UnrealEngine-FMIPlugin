// Fill out your copyright notice in the Description page of Project Settings.


#include "FmuActorComponent.h"
#include "GameFramework/Actor.h"
#include "XmlFile.h"
#include "unzipper.hpp"


// Sets default values for this component's properties
UFmuActorComponent::UFmuActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	mPath.FilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() + "../test.fmu");
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

	// Extract from an .fmu file
	{
		if (mPath.FilePath.IsEmpty())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Path to .fmu is empty."));
			return;
		}
		FString fullPath = FPaths::ConvertRelativePathToFull(mPath.FilePath);
		std::string sPath = TCHAR_TO_UTF8(*fullPath);
		size_t lastindex = sPath.find_last_of(".");
		mUnzipDir = sPath.substr(0, lastindex);
		unzip(sPath, mUnzipDir);
	}

	// These should be populated from the extracted ModelDescription.xml
	{
		std::string xmlFile = mUnzipDir + "/modelDescription.xml";
		FString fXmlFile = UTF8_TO_TCHAR(xmlFile.c_str());
		FXmlFile model(fXmlFile, EConstructMethod::ConstructFromFile);
		FXmlNode *root = model.GetRootNode();
		FXmlNode *defaultExperiment = root->FindChildNode("DefaultExperiment");
		FXmlNode *modelVariables = root->FindChildNode("ModelVariables");
		TArray<FXmlNode*> nodes = modelVariables->GetChildrenNodes();
		for (FXmlNode* node : nodes)
		{
			FString key = node->GetAttribute("name");
			int value = FCString::Atoi(*node->GetAttribute("valueReference"));
			mValRefMap.Add(key, value);
		}
		mGuid = TCHAR_TO_UTF8(*root->GetAttribute("guid"));
		mModelIdentifier = TCHAR_TO_UTF8(*root->GetAttribute("modelName"));;
		mInstanceName = "instance";
		mStartTime = FCString::Atof(*defaultExperiment->GetAttribute("startTime"));
		mStopTime = FCString::Atof(*defaultExperiment->GetAttribute("stopTime")) * StopTimeMultiplier;
		mStepSize = 0.1;
		mTolerance = FCString::Atof(*defaultExperiment->GetAttribute("tolerance"));;
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
	float x = mFmu->getReal(mValRefMap["x"]) * DistanceMultiplier[0] + mStartLocation[0];
	float y = mFmu->getReal(mValRefMap["y"]) * DistanceMultiplier[1] + mStartLocation[1];
	float z = mFmu->getReal(mValRefMap["z"]) * DistanceMultiplier[2] + mStartLocation[2];
	mNewLocation = {x , y , z};
	GetOwner()->SetActorLocation(mNewLocation);
}

