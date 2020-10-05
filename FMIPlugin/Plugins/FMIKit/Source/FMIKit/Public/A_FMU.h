// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FMU2.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A_FMU.generated.h"

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

	//// Called when actor is created or any updates are made to it
	//virtual void OnConstruction() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//User controls
	// float StartTime = 0.f; //update to default fmu xml
	// float StopTime = 1.f; // update to default fmu xml
	// bool bRestart = false;
	// bool bStart = false;
	// bool bStop = false;
	// bool bPause = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float SpeedMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FVector DistanceMultiplier = { 1.f,1.f,1.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float StopTimeMultiplier = 1.f;

};
