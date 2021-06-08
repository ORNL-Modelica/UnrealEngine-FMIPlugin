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