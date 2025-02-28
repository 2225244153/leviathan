// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/RootMotionSource.h"
#include "Leviathan/AbilitySystem/Abilities/GHAnimSkillAbility.h"
#include "SkillKnockComponent.generated.h"

USTRUCT(BlueprintType)
struct FKnockForceMoveInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsAdditive = false;

	/**
	 * @brief 指定曲线名称， 默认 "Knock"
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName AnimCurveForStrengthName = FName("Knock");

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ERootMotionFinishVelocityMode VelocityOnFinishMode = ERootMotionFinishVelocityMode::MaintainLastRootMotionVelocity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector SetVelocityOnFinish;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ClampVelocityOnFinish = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool EnableGravity = false;

	/**
	 * @brief 方向
	 */
	UPROPERTY(BlueprintReadOnly)
	FVector Direction;

	/**
	 * @brief 强度
	 */
	UPROPERTY(BlueprintReadOnly)
	float Strength = 0.f;
};

USTRUCT(BlueprintType)
struct FKnockInfo
{
	GENERATED_BODY()

	/*
	 * 方向
	 */
	UPROPERTY(BlueprintReadOnly)
	FVector Direction;

	/*
	 * 力量
	 */
	UPROPERTY(BlueprintReadOnly)
	float Strength;

	/*
	 * 强制旋转
	 */
	UPROPERTY(BlueprintReadOnly)
	bool bForceChangeDirection;


	/*强制移动*/
	UPROPERTY(BlueprintReadOnly)
	bool bForceMove;

	/*
	 * 强制移动信息
	 */
	UPROPERTY(BlueprintReadOnly)
	FKnockForceMoveInfo KnockForceMoveInfo;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LEVIATHAN_API USkillKnockComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USkillKnockComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	/**
     * @brief 实际激活硬直
     * @param knock_info 
     * @param dir 
     * @param strength 
     */
	void ActivateKnock(const FKnockInfo& knock_info, const FVector& dir, const float& strength);

	/**
	 * @brief 处理强制转身和位移
	 * @param knock_info 
	 * @param dir 
	 * @param strength 
	 */
	void ForceRotateAndMove(const FKnockInfo& knock_info, const FVector& dir, const float& strength);

	void HandleSkillKnock();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGHAnimSkillAbility> DefaultKnockAbility;

	FKnockInfo CurrentKnockInfo;
};
