// Fill out your copyright notice in the Description page of Project Settings.

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

	UFUNCTION(BlueprintCallable)
		float GetReal(FString Name);
	UFUNCTION(BlueprintCallable)
		void DoStep(float StepSize);
	UFUNCTION(BlueprintCallable)
		bool ControlStep(float DeltaTime);
	UFUNCTION(BlueprintCallable)
		void SetReal(FString Name, float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		bool mAutoSimulateTick = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		FFilePath mPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		float mSpeedMultiplier = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		TMap<FName, FModelVariables> mModelVariables;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		TArray<FString> mStoredVariables;
	UPROPERTY(BlueprintReadWrite, Category = "FMU Settings")
		TMap<FString, float> mResults;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		bool bUseXMLExperimentSettings = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		float mStartTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		float mStopTime = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		float mStepSize = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		float mTolerance = 1e-4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		bool mPause = false;
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
