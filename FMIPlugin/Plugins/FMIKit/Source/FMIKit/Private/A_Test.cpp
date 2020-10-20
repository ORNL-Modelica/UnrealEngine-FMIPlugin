// Fill out your copyright notice in the Description page of Project Settings.

#include "A_Test.h"

// Sets default values
AA_Test::AA_Test()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

#if WITH_EDITOR
void AA_Test::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	//Super::PostEditChangeProperty(e);

	if (e.MemberProperty->GetFName().ToString() == TEXT("mPath"))
	{
		mUnzipDir = "hello";
	}
}
#endif

// Called when the game starts or when spawned
void AA_Test::BeginPlay()
{
	//SetActorTickInterval(1.f);
	Super::BeginPlay();

	FString  test = mUnzipDir;
	FString  cat = mUnzipDir;
}