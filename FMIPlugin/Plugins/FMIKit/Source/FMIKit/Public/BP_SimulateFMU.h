// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BP_SimulateFMU.generated.h"

/**
 * 
 */
UCLASS()
class FMIKIT_API UBP_SimulateFMU : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
    UFUNCTION(BlueprintCallable, Category = "My Nodes")
    //UFUNCTION(BlueprintPure, meta = (DisplayName = "Square Number (Fancy~)", CompactNodeTitle = "Square"), Category = "My Nodes")
    static float SimulateFMU(const float In);
};
