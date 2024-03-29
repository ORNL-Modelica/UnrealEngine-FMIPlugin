/*
Copyright 2021, UT-Battelle, LLC]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "A_FMU.h"

#include "XmlFile.h"
#include "..\Public\A_FMU.h"

#ifdef WIN32
#include "Windows.h"
#define cmd(a) WinExec(a, SW_HIDE)
#define comparison < 32

#include <direct.h>
#define mkdir _mkdir
#else
#define cmd(a) system(a)
#define comparison != 0
#endif

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

	if (e.MemberProperty->GetFName().ToString() == TEXT("PathFMU"))
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
	
	Initialize();
}

void AA_FMU::Initialize()
{
	mbLoaded = false;
	mTimeNow = 0.0;
	mTimeLast = 0.0;
	mFMUTime = mStartTime;

	// Believe this is no longer needed
	//AA_FMU::DestroyFMU();

	// if(!mFmu) // This line with the new fmikit.... prevents (we think) issues with locked dll file. However, it messes up autolooping... need to find a better solution
	mFmu = std::make_unique<fmikit::FMU2Slave>(TCHAR_TO_UTF8(*mGuid), TCHAR_TO_UTF8(*mModelIdentifier), TCHAR_TO_UTF8(*mUnzipDir), TCHAR_TO_UTF8(*mInstanceName));
	mFmu->instantiate(true);

	// Initial values seem to be required to be set at multiple places
	// Line 1180 - under "if initialize:" https://github.com/CATIA-Systems/FMPy/blob/c01eb7b45a8c0542986d6692458f8c81df425587/fmpy/simulation.py#L1163
	//SetInitialValues();
	mFmu->setupExperiment(true, mTolerance, mStartTime, true, mStopTime);
	SetInitialValues();
	mFmu->enterInitializationMode();
	SetInitialValues();
	mFmu->exitInitializationMode();
	
	mbLoaded = true;
	UE_LOG(LogTemp, Display, TEXT("Initialization of FMU complete: %s"), *mPath.FilePath);
}

void AA_FMU::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AA_FMU::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// This is an option to let the user have an auto simulated model instead of blueprint control. Having issues
	if (mAutoSimulateTick && !mPause)
	{
		try
		{
			if (mLoop && (mFMUTime > mStopTime))
			{
				Initialize();
			}

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
		catch(std::exception e)
		{
			FString errorMsg(e.what());
			UE_LOG(LogTemp, Error, TEXT("%s"), *errorMsg);
		}
	}
}

void AA_FMU::ExtractFMU()
{
	mPath = PathFMU;
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
	
	if (FPaths::IsRelative(*PathFMU.FilePath))
	{
		mPath.FilePath = FPaths::Combine(FPaths::ProjectContentDir(), PathFMU.FilePath);
		mPath.FilePath = FPaths::ConvertRelativePathToFull(mPath.FilePath);
	}

	if (!FPaths::FileExists(*mPath.FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid mPath. %s not found."), *mPath.FilePath);
		return;
	}

	// Gather parts of FMU path
	FString pathPart, filename, extension;
	FPaths::Split(mPath.FilePath, pathPart, filename, extension);
	std::string sPath = TCHAR_TO_UTF8(*mPath.FilePath);

	// Define path for extracted the FMU
	FString projPath = FPaths::ProjectDir();
	FString exDir = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*projPath) + "fmus/"; // Directory to extract FMU
	
	if (!(FPaths::DirectoryExists(exDir)))	{
		std::string tempString = TCHAR_TO_UTF8(*exDir);
		mkdir(tempString.c_str());
	}
	FString actorName = GetName(); // added in case of multiple FMUs
	mUnzipDir = exDir + filename + actorName;

	// Extract the FMU: attempt to use unzip, then 7z, then tar
	std::string dir = TCHAR_TO_UTF8(*mUnzipDir);
	std::string exe = "unzip " + sPath + " -d " + dir;
	bool success = true;
	if (cmd(exe.c_str()) comparison) {
		exe = "7z x \"" + sPath + "\" -aoa -o\"" + dir + "\"";
		if (cmd(exe.c_str()) comparison) {
			mkdir(dir.c_str());
			exe = "tar -xf \"" + sPath + "\" -C \"" + dir + "\"";
			success = !(cmd(exe.c_str()) comparison);
		}
	}

	// TODO: Dual maintenance with FmuActorComponent.cpp
	// TODO: Add jar, minizip or other as unzip option? Use CreateProcess() instead of WinExec()?
	FString extracted = success ? "Extracted" : "Failed to extract";
	FString msg(exe.c_str());
	UE_LOG(LogTemp, Display, TEXT("%s fmu using command: %s"), *extracted, *msg);

	// there seems to be a race condition for unzipping where the above messages and ParseXML sometimes occur before the fmu is done unzipping...
	ParseXML();
}

void AA_FMU::ParseXML()
{
	FString xmlFile = mUnzipDir + "/modelDescription.xml";

	// Add loop to avoid race conditions where the unzip process has not completed
	int counter = 0;
	while (!FPaths::FileExists(*xmlFile) && counter < 5) {
		UE_LOG(LogTemp, Error, TEXT("Attempting to parse XML"), *xmlFile);
		Sleep(100);
		counter++;
	}

	if (!FPaths::FileExists(*xmlFile))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid mPath. %s not found. Make sure a supported unzip tool is on the PATH"), *xmlFile);
		return;
	}

	// fmiModelDescription (root)
	FXmlFile model(xmlFile, EConstructMethod::ConstructFromFile);
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
		mStepSize = FCString::Atof(*defaultExperiment->GetAttribute("stepSize"));
		mTolerance = FCString::Atof(*defaultExperiment->GetAttribute("tolerance"));

		// Check to ensure defaults exists that make sense. 0 is returned if the attribute is not found in the xml.
		if (mStopTime == 0)
		{
			mStopTime = 1.0;
		}
		if (mStepSize == 0)
		{
			mStepSize = 0.1;
		}
		if (mTolerance == 0)
		{
			mTolerance = 1e-4;
		}
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

float AA_FMU::GetReal(FString Name)
{
	if (!mModelVariables.Contains(FName(Name)))
		return std::numeric_limits<float>::lowest();
	if (!mbLoaded)
		return std::numeric_limits<float>::lowest();
	return mFmu->getReal(mModelVariables[FName(Name)].ValueReference);
}

void AA_FMU::DoStep(float StepSize)
{
	if (!mbLoaded)
		return;
	mFmu->doStep(StepSize);
}

void AA_FMU::SetReal(FString Name, float Value)
{
	if (!mModelVariables.Contains(FName(Name)))
		return;
	if (!mbLoaded)
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

void AA_FMU::SetInitialValues()
{
	if (!mbLoaded)
		return;
	for (const TPair<FString, float>& pair : mInitialValues)
	{
		AA_FMU::SetReal(pair.Key, pair.Value);
	}
}