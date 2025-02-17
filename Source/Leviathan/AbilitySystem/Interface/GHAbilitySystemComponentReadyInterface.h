// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GHAbilitySystemComponentReadyInterface.generated.h"

class UGHAbilitySystemComponent;

UINTERFACE(BlueprintType)
class LEVIATHAN_API UGHAbilitySystemComponentReadyInterface: public UInterface
{
	GENERATED_BODY()
};

class IGHAbilitySystemComponentReadyInterface
{
	GENERATED_BODY()

public:
	/**
	 * @brief 当角色初始化完毕后，会调用该方法通知组件。
	 */
	virtual void OnAbilitySystemComponentReady() = 0;


	/**
	 * @brief 
	 * @param AbilitySystemComponent 
	 * @param Actor 
	 */
	virtual void SetAbilitySystemComponent(UGHAbilitySystemComponent *AbilitySystemComponent ,AActor *Actor) = 0;
};