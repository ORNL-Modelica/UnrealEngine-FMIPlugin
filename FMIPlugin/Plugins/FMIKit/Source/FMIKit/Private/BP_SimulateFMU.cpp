// Fill out your copyright notice in the Description page of Project Settings.

#include "BP_SimulateFMU.h"
#include <string>
#include "FMU2.h"
//#include "fmi2Functions.h"

float UBP_SimulateFMU::SimulateFMU(const float In) {
	UE_LOG(LogTemp, Warning, TEXT("UBP_SimulateFMU::SimulateFMU"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("UBP_SimulateFMU::SimulateFMU"));

	return FMath::Pow(In, 2);
}