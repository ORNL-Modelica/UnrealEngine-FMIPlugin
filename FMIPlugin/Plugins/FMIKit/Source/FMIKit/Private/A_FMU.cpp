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
}

// Called when actor is created or any updates are made to it
void AA_FMU::OnConstruction(const FTransform& Transform)
{
	UE_LOG(LogTemp, Warning, TEXT("test"));
	ExtractFMU();
	ParseXML();
}

// Called when the game starts or when spawned
void AA_FMU::BeginPlay()
{
	//SetActorTickInterval(1.f);
	Super::BeginPlay();
	
	mFmu = new fmikit::FMU2Slave(mGuid, mModelIdentifier, mUnzipDir, mInstanceName);
    mFmu->instantiate(true);
    mFmu->setupExperiment(true, mTolerance, mStartTime, true, mStopTime);
    // More complex models may need to change values here
    mFmu->enterInitializationMode();
    // or here
    mFmu->exitInitializationMode();
	mLoaded = true;

	UE_LOG(LogTemp, Error, TEXT("%s"), *mPath.FilePath);
}

// Called every frame
void AA_FMU::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!mLoaded)
		return;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hello"));
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
    // Clear existing map
    mValRefMap.Empty();

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
	mStopTime = FCString::Atof(*defaultExperiment->GetAttribute("stopTime")) * mStopTimeMultiplier;
	mTolerance = FCString::Atof(*defaultExperiment->GetAttribute("tolerance"));;

	// ModelVariables
	FXmlNode* modelVariables = root->FindChildNode("ModelVariables");
	TArray<FXmlNode*> nodes = modelVariables->GetChildrenNodes();
	for (FXmlNode* node : nodes)
	{
        FString key = node->GetAttribute("name");
        int value = FCString::Atoi(*node->GetAttribute("valueReference"));
		mValRefMap.Add(key, value);
	}

	// ModelStructure
	// -

}

float AA_FMU::GetReal(unsigned int valRef)
{
    return mFmu->getReal(valRef);
}

void AA_FMU::DoStep(float time)
{
    mFmu->doStep(time);
}

