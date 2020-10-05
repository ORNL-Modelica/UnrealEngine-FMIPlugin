// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Map.h"
#include "FMU2.h"
#include "Engine/EngineTypes.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FmuActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FMIKIT_API UFmuActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFmuActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		float SpeedMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		FVector DistanceMultiplier = {1.f,1.f,1.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		float StopTimeMultiplier = 1.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FFilePath mPath;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapsAndSets")
        TMap<FString, int> mValRefMap;

private:
	fmikit::FMU2Slave *mFmu = nullptr;

	std::string mUnzipDir;
	std::string mGuid;
	std::string mModelIdentifier;
	std::string mInstanceName;
	fmi2Real mStartTime;
	fmi2Real mStopTime;
	fmi2Real mStepSize;
	fmi2Real mTolerance;
	fmi2Real mTimeLast;
	fmi2Real mTimeNow;
	bool mLoaded = false;

	FVector mStartLocation;
	FVector mNewLocation;
};
