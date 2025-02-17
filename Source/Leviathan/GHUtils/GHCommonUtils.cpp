// Fill out your copyright notice in the Description page of Project Settings.


#include "GHCommonUtils.h"

float UGHCommonUtils::CalcDistance(AActor* myself, AActor* target, bool dis2D)
{
	if (myself == nullptr || target == nullptr)
	{
		return 0.f;
	}

	float dis  = 0;
	if (dis2D)
	{
		dis = FVector::Dist2D(myself->GetActorLocation(), target->GetActorLocation());
	}
	else
	{
		dis = FVector::Dist(myself->GetActorLocation(), target->GetActorLocation());
	}
	return dis;
}

float UGHCommonUtils::Calc2DAngleByForward(AActor* myself, AActor* target)
{
	if (myself == nullptr || target == nullptr)
	{
		return 0.f;
	}
	FVector selfLoc = myself->GetActorLocation();
	FVector targetLoc = target->GetActorLocation();
	FVector selfForwardVec = myself->GetActorForwardVector();
	FVector targetVec = (targetLoc - selfLoc).GetSafeNormal2D();
	float angle = 180.f / PI * FMath::Acos(FVector::DotProduct(selfForwardVec, targetVec));
	return angle;
}
