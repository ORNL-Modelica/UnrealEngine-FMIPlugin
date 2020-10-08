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
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

	// Called when actor is created or any updates are made to it
	virtual void OnConstruction(const FTransform& Transform) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void ExtractFMU();
	void ParseXML();

    UFUNCTION(BlueprintCallable)
    float GetReal(int valRef);
    UFUNCTION(BlueprintCallable)
    void DoStep(float time);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	    FFilePath mPath = { FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() + "../test.fmu") };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	    float mSpeedMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	    FVector mDistanceMultiplier = { 1.f,1.f,1.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		TMap<FName, FModelVariables> mModelVariables;

private:
	fmikit::FMU2Slave *mFmu = nullptr;

	std::string mUnzipDir;
	std::string mGuid;
	std::string mModelIdentifier;
    std::string mFMIVersion;
	std::string mInstanceName = "instance";
	fmi2Real mStartTime = 0.;
	fmi2Real mStopTime = 1.;
	fmi2Real mStepSize = 1.;
	fmi2Real mTolerance = 1e-4;
	bool mLoaded = false;	
};
