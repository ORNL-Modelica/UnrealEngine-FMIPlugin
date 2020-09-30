// Fill out your copyright notice in the Description page of Project Settings.

#include "BP_SimulateFMU.h"

#include "FMU2.h"

float UBP_SimulateFMU::SimulateFMU(const float In) {
	UE_LOG(LogTemp, Warning, TEXT("DemoText"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));

	return FMath::Pow(In, 2);
}