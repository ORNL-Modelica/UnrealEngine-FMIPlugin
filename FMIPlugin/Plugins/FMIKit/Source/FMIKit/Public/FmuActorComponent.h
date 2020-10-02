// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

private:
	UPROPERTY(EditAnywhere)
	FString mPath = "../test";
	//FFilePath mPath;

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
};
