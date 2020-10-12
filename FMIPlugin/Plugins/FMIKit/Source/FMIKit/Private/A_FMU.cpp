// Fill out your copyright notice in the Description page of Project Settings.


#include "A_FMU.h"

#include "XmlFile.h"
#include "unzipper.hpp"

// Sets default values
AA_FMU::AA_FMU()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    ExtractFMU();
    ParseXML();
	mResults.Empty();
}

// Called when actor is created or any updates are made to it
void AA_FMU::OnConstruction(const FTransform& Transform)
{
	//UE_LOG(LogTemp, Warning, TEXT("test"));

}

#if WITH_EDITOR
void AA_FMU::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);
	
	if (e.MemberProperty->GetFName().ToString() == TEXT("mPath"))
	{
		ExtractFMU();
		ParseXML();
	}

	if (mAutoSimulateTick && e.MemberProperty->GetFName().ToString() == TEXT("mStoreVariables"))
	{
		mResults.Empty();
	}

	//if (e.MemberProperty->GetFName().ToString() == TEXT("mPause"))
	//{
	//	mPause = !mPause;
	//}
}
#endif

// Called when the game starts or when spawned
void AA_FMU::BeginPlay()
{
	//SetActorTickInterval(1.f);
	Super::BeginPlay();
	
	mFmu = new fmikit::FMU2Slave(mGuid, mModelIdentifier, mUnzipDir, mInstanceName);
    mFmu->instantiate(true);
    mFmu->setupExperiment(true, mTolerance, mStartTime, true, mStopTime);
    mFmu->enterInitializationMode();
    mFmu->exitInitializationMode();
	mLoaded = true;

	UE_LOG(LogTemp, Display, TEXT("Initialization of FMU complete: %s"), *mPath.FilePath);
}

// Called every frame
void AA_FMU::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// This is an option to let the user have an auto simulated model instead of blueprint control. Having issues
	if (mAutoSimulateTick && !mPause)
	{
		if (ControlStep(DeltaTime))
		{
			mFmu->doStep(mStepSize);
		}
		else
		{
			return;
		}
		
		for (FString Key : mStoredVariables)
		{
			if (mResults.Contains(Key))
			{
				mResults[Key] = mFmu->getReal(mModelVariables[FName(Key)].ValueReference);
			}
			else
			{
				mResults.Add(Key, mFmu->getReal(mModelVariables[FName(Key)].ValueReference));
			}
		}
	}
}

void AA_FMU::ExtractFMU()
{
	if (mPath.FilePath.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Path to .fmu is empty."));
		return;
	}
	std::string sPath = TCHAR_TO_UTF8(*mPath.FilePath);
	size_t lastindex = sPath.find_last_of(".");
	mUnzipDir = sPath.substr(0, lastindex);
	unzip(sPath, mUnzipDir);
}

void AA_FMU::ParseXML()
{
	std::string xmlFile = mUnzipDir + "/modelDescription.xml";
	FString fXmlFile = UTF8_TO_TCHAR(xmlFile.c_str());
	FXmlFile model(fXmlFile, EConstructMethod::ConstructFromFile);
	
	// fmiModelDescription (root)
	FXmlNode* root = model.GetRootNode();
	mFMIVersion = TCHAR_TO_UTF8(*root->GetAttribute("fmiVersion"));
	mModelIdentifier = TCHAR_TO_UTF8(*root->GetAttribute("modelName"));;
	mGuid = TCHAR_TO_UTF8(*root->GetAttribute("guid")); 

	// CoSimulation
	// -

	// DefaultExperiment
	FXmlNode* defaultExperiment = root->FindChildNode("DefaultExperiment");
	mStartTime = FCString::Atof(*defaultExperiment->GetAttribute("startTime"));
	mStopTime = FCString::Atof(*defaultExperiment->GetAttribute("stopTime"));
	mTolerance = FCString::Atof(*defaultExperiment->GetAttribute("tolerance"));

	// ModelVariables
	FXmlNode* modelVariables = root->FindChildNode("ModelVariables");
	TArray<FXmlNode*> nodes = modelVariables->GetChildrenNodes();

	// Clear existing values in TMap
	mModelVariables.Empty();

	// Populate TMap
	for (FXmlNode* node : nodes)
	{
		struct FModelVariables ModelVariables;
		ModelVariables.ValueReference = FCString::Atoi(*node->GetAttribute("valueReference"));
		FString key = node->GetAttribute("name");
		mModelVariables.Add(FName(key), ModelVariables);
		// may need to add logic to handle non integer values if needeed in the future. Ignore them or assign them in some other way.
	}

	// ModelStructure
	// -

	UE_LOG(LogTemp, Display, TEXT("XML parsing complete for: %s"), UTF8_TO_TCHAR(mModelIdentifier.c_str()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("XML parsing complete for: %s"), *tests); // Does not work in VS2019
}

float AA_FMU::GetReal(FString Name)
{
    return mFmu->getReal(mModelVariables[FName(Name)].ValueReference);
}

void AA_FMU::DoStep(float StepSize)
{
    mFmu->doStep(StepSize);
}

void AA_FMU::SetReal(FString Name, float Value)
{
	mFmu->setReal(mModelVariables[FName(Name)].ValueReference, Value);
}

bool AA_FMU::ControlStep(float DeltaTime)
{
	if (!mLoaded)
		return false;

	mTimeNow += DeltaTime;
	if (!(mTimeNow > mTimeLast + mStepSize / mSpeedMultiplier))
		return false;

	if (mTimeLast >= mStopTime / mSpeedMultiplier)
		return false;

	mTimeLast += mStepSize / mSpeedMultiplier;

	return true;
}