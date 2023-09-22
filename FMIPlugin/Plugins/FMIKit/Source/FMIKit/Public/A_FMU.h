/*
Copyright 2021, UT-Battelle, LLC

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

#pragma once

#include "Containers/Map.h"
#include "Engine/DataTable.h"
#include "FMU2.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A_FMU.generated.h"

USTRUCT()
struct FModelVariables : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int ValueReference;
};

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//virtual void BeginDestroy() override;
	//virtual void FinishDestroy() override;

	// Called when actor is created or any updates are made to it
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitProperties() override;
	virtual void PostLoad() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ExtractFMU();
	void ParseXML();
	void GetModelDescription();
	void SetInitialValues();

	// Function for instantiating the FMU (can be used to reset the FMU simulation as well).
	UFUNCTION(BlueprintCallable)
		void Initialize();
	// Return the real variable of the specified name.
	UFUNCTION(BlueprintCallable)
		float GetReal(FString Name);
	// Function to solver the FMU to the specified time step
	UFUNCTION(BlueprintCallable)
		void DoStep(float StepSize);
	// Control logic to sync real time and fmu time together. Typically called before DoStep().
	UFUNCTION(BlueprintCallable)
		bool ControlStep(float DeltaTime);
	// Change an FMU variable value.
	UFUNCTION(BlueprintCallable)
		void SetReal(FString Name, float Value);

	// Terminate and free the FMU instance
	UFUNCTION(BlueprintCallable)
		void DestroyFMU();

	// Use to specify if the internal logic is desired to run the FMU. Else control FMU solution manually.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		bool mAutoSimulateTick = false;
	// Full file path of the *.fmu file
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		FFilePath PathFMU;
	UPROPERTY(BlueprintReadOnly, Category = "FMU Settings")
		FFilePath mPath;
	// Change the simulation speed of the FMU relative to real time.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		float mSpeedMultiplier = 1.0f;
	// Variables available in the XML for inputs/outputs
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		TMap<FName, FModelVariables> mModelVariables;
	// Specify the variables to store for access. See mModelVariables for available variables.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		TArray<FString> mStoredVariables;
	// Value of stored variables. Indexed in the same order as mStoredVariables.
	UPROPERTY(BlueprintReadWrite, Category = "FMU Settings")
		TMap<FString, float> mResults;
	// Specify the initial values of variables. See mModelVariables for potentially available variables.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		TMap<FString, float> mInitialValues;

	// Toggle to overwrite the editor specified settings with those found in FMU model description XML.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		bool bUseXMLExperimentSettings = false;
	// Start time of the FMU simulation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		float mStartTime = 0.f;
	// Stop time of the FMU simulation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		float mStopTime = 1.f;
	// Time step (s) of the FMU simulation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		float mStepSize = 0.1f;
	// FMU solver tolerance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		float mTolerance = 1e-4;
	// Variable to pause the FMU simulation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		bool mPause = false;
	// Toggle to have the simulation reset and restart when the mStopTime is reached
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		bool mLoop = false;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
	//bool mbPrintFMUTime = false;

	UPROPERTY(BlueprintReadOnly, Category = "FMU Settings")
		FString mUnzipDir;
	UPROPERTY(BlueprintReadOnly, Category = "FMU Settings")
		FString mGuid;
	UPROPERTY(BlueprintReadOnly, Category = "FMU Settings")
		FString mModelIdentifier;
	UPROPERTY(BlueprintReadOnly, Category = "FMU Settings")
		FString mFMIVersion;
	UPROPERTY(BlueprintReadOnly, Category = "FMU Settings")
		FString mInstanceName = "instance";

	UPROPERTY(BlueprintReadOnly, Category = "FMU Settings")
		float mFMUTime;
private:
	fmikit::FMU2Slave* mFmu = nullptr;

	bool mbLoaded = false;
	
	fmi2Real mTimeLast;
	fmi2Real mTimeNow;
	
};
