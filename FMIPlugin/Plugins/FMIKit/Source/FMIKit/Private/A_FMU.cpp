// Fill out your copyright notice in the Description page of Project Settings.


#include "A_FMU.h"

#include "XmlFile.h"
#include "unzipper.hpp"
#include "..\Public\A_FMU.h"

// Sets default values
AA_FMU::AA_FMU()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when actor is created or any updates are made to it
void AA_FMU::OnConstruction(const FTransform& Transform)
{

}

#if WITH_EDITOR
void AA_FMU::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	if (e.MemberProperty->GetFName().ToString() == TEXT("mPath"))
	{
		ExtractFMU();
		mResults.Empty();
	}
	if (mAutoSimulateTick && e.MemberProperty->GetFName().ToString() == TEXT("mStoreVariables"))
	{
		mResults.Empty();
	}
	if (e.MemberProperty->GetFName().ToString() == TEXT("bUseXMLExperimentSettings"))
	{
		if (bUseXMLExperimentSettings)
		{
			ParseXML();
		}
	}
}
#endif

void AA_FMU::PostInitProperties()
{
	Super::PostInitProperties();
}

void AA_FMU::PostLoad()
{
	Super::PostLoad();

	ExtractFMU();
	mResults.Empty();
}

// Called when the game starts or when spawned
void AA_FMU::BeginPlay()
{

	// Prevent crashing due to missing extracted fmu
	bool bFMUExtractedExists = FPaths::FileExists(mUnzipDir + "/modelDescription.xml");
	if (!bFMUExtractedExists)
	{
		UE_LOG(LogTemp, Error, TEXT("Initialization of FMU failed. Try checking that the .fmu exists. Currently looking for %s"), *mUnzipDir);
		return;
	}

	//SetActorTickInterval(1.f);
	Super::BeginPlay();

	mFmu = new fmikit::FMU2Slave(TCHAR_TO_UTF8(*mGuid), TCHAR_TO_UTF8(*mModelIdentifier), TCHAR_TO_UTF8(*mUnzipDir), TCHAR_TO_UTF8(*mInstanceName));
	mFmu->instantiate(true);
	mFmu->setupExperiment(true, mTolerance, mStartTime, true, mStopTime);
	mFmu->enterInitializationMode();
	mFmu->exitInitializationMode();
	mbLoaded = true;

	mFMUTime = mStartTime;

	UE_LOG(LogTemp, Display, TEXT("Initialization of FMU complete: %s"), *mPath.FilePath);
}

void AA_FMU::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogTemp, Log, TEXT("cleanup worked"));
	//mFmu = new fmikit::FMU2Slave(TCHAR_TO_UTF8(*mGuid), TCHAR_TO_UTF8(*mModelIdentifier), TCHAR_TO_UTF8(*mUnzipDir), TCHAR_TO_UTF8(*mInstanceName));
	delete mFmu;
	Super::EndPlay(EndPlayReason);
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
			else if (mModelVariables.Contains(FName(Key)))
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
		UE_LOG(LogTemp, Warning, TEXT("mPath to .fmu is empty."));
		return;
	}
	if (FPaths::GetExtension(*mPath.FilePath, false) != TEXT("fmu"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid mPath. It does not contain a `.fmu` extension."));
		return;
	}
	if (!FPaths::FileExists(*mPath.FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid mPath. %s not found."), *mPath.FilePath);
		return;
	}

    mPath.FilePath = FPaths::ConvertRelativePathToFull(mPath.FilePath);
	std::string sPath = TCHAR_TO_UTF8(*mPath.FilePath);
	size_t lastindex = sPath.find_last_of(".");
	mUnzipDir = UTF8_TO_TCHAR(sPath.substr(0, lastindex).c_str());
	unzip(sPath, TCHAR_TO_UTF8(*mUnzipDir));

	ParseXML();
}

void AA_FMU::ParseXML()
{
	FString xmlFile = mUnzipDir + "/modelDescription.xml";

	if (!FPaths::FileExists(*xmlFile))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid mPath. %s not found."), *xmlFile);
		return;
	}

	FXmlFile model(xmlFile, EConstructMethod::ConstructFromFile);

	// fmiModelDescription (root)
	FXmlNode* root = model.GetRootNode();
	mFMIVersion = *root->GetAttribute("fmiVersion");
	mModelIdentifier = *root->GetAttribute("modelName");
	mGuid = *root->GetAttribute("guid");

	// CoSimulation
	// -

	// DefaultExperiment
	if (bUseXMLExperimentSettings)
	{
		FXmlNode* defaultExperiment = root->FindChildNode("DefaultExperiment");
		mStartTime = FCString::Atof(*defaultExperiment->GetAttribute("startTime"));
		mStopTime = FCString::Atof(*defaultExperiment->GetAttribute("stopTime"));
		mTolerance = FCString::Atof(*defaultExperiment->GetAttribute("tolerance"));
	}

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

	UE_LOG(LogTemp, Display, TEXT("XML parsing complete for: %s"), *mModelIdentifier);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("XML parsing complete for: %s"), *tests); // Does not work in VS2019
}

void AA_FMU::GetModelDescription()
{
	if (mPath.FilePath.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("mPath to .fmu is empty."));
		return;
	}

	std::string sPath = TCHAR_TO_UTF8(*mPath.FilePath);
	size_t lastindex = sPath.find_last_of(".");
	mUnzipDir = UTF8_TO_TCHAR(sPath.substr(0, lastindex).c_str());

	FString xmlFile = mUnzipDir + "/modelDescription.xml";
	FXmlFile model(xmlFile, EConstructMethod::ConstructFromFile);

	// fmiModelDescription (root)
	FXmlNode* root = model.GetRootNode();
	mFMIVersion = *root->GetAttribute("fmiVersion");
	mModelIdentifier = *root->GetAttribute("modelName");
	mGuid = *root->GetAttribute("guid");
}

float AA_FMU::GetReal(FString Name)
{
	if (!mModelVariables.Contains(FName(Name)))
		return std::numeric_limits<float>::lowest();
	return mFmu->getReal(mModelVariables[FName(Name)].ValueReference);
}

void AA_FMU::DoStep(float StepSize)
{
	mFmu->doStep(StepSize);
}

void AA_FMU::SetReal(FString Name, float Value)
{
	if (!mModelVariables.Contains(FName(Name)))
		return;
	mFmu->setReal(mModelVariables[FName(Name)].ValueReference, Value);
}

bool AA_FMU::ControlStep(float DeltaTime)
{
	if (!mbLoaded)
		return false;

	mTimeNow += DeltaTime;
	if (!(mTimeNow > mTimeLast + mStepSize / mSpeedMultiplier))
		return false;

	if (mTimeLast >= mStopTime / mSpeedMultiplier)
		return false;

	mTimeLast += mStepSize / mSpeedMultiplier;
	mFMUTime += mStepSize;

	//if (mbPrintFMUTime)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("XML parsing complete for: %f"), mFMUTime);
	//}
	return true;
}
