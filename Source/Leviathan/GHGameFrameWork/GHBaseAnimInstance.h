// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GHBaseAnimInstance.generated.h"

class AGHBaseCharacter;

/**
 * 
 */
UCLASS()
class LEVIATHAN_API UGHBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UGHBaseAnimInstance();
	
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void BeginDestroy() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bMoving;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float MoveSpeed;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)//用于动画表现的方向
	float Direction;

protected:
	UPROPERTY()
	AGHBaseCharacter* BaseCharacter;
};
