// Fill out your copyright notice in the Description page of Project Settings.

//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
//#include "A_Test.generated.h"
//
//UCLASS()
//class FMIKIT_API AA_Test : public AActor
//{
//	GENERATED_BODY()
//	
//public:	
//	// Sets default values for this actor's properties
//	AA_Test();
//
//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
//		FFilePath mPath = { FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() + "../ies.fmu") };
//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;
//
//};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A_Test.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FMIKIT_API AA_Test : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AA_Test();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMU Settings")
		FFilePath mPath;

// With UPROPERTY() - this works as expected
	//UPROPERTY(BlueprintReadOnly, Category = "FMU Settings")
	//FString mUnzipDir;

// Without UPROPERTY() -> this gets reset to default "Empty"
	//FString mUnzipDir;
private:

// As private variable, UPROPERTY() not permitted -> this gets reset to default "Empty"
	FString mUnzipDir;
};
