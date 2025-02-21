// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GHCommonUtils.generated.h"

/**
 * 
 */
UCLASS()
class LEVIATHAN_API UGHCommonUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*
	 * 计算actor之间的距离
	 */
	UFUNCTION(BlueprintCallable, Category = "GH|Utils")
	static float CalcDistance(AActor* myself, AActor* target, bool dis2D = true);
	UFUNCTION(BlueprintCallable, Category = "GH|Utils")
	static float CalcDistance2(AActor* character, FVector& loc, bool dis2D = true);
	UFUNCTION(BlueprintCallable, Category = "GH|Utils")
	static float CalcDistance3(FVector& loc1, FVector& loc2, bool dis2D = true);
	/*
	 * 计算目标基于自身正前方的角度
	 */
	UFUNCTION(BlueprintCallable, Category = "GH|Utils")
	static float Calc2DAngleByForward(AActor* myself, AActor* target);

	/*
	 *	计算权重
	 */
	UFUNCTION(BlueprintCallable, Category = "GH|Utils")
	static int32 RandomIntegerByWeight(TArray<int32>& weight);
};
