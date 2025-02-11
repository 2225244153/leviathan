// Fill out your copyright notice in the Description page of Project Settings.


#include "GHBaseAnimInstance.h"

#include "GHBaseCharacter.h"
#include "KismetAnimationLibrary.h"

UGHBaseAnimInstance::UGHBaseAnimInstance() : bMoving(false), MoveSpeed(0), Direction(0), BaseCharacter(nullptr)
{
}

void UGHBaseAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	BaseCharacter = Cast<AGHBaseCharacter>(GetOwningActor());
}

void UGHBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (BaseCharacter == nullptr)
	{
		return;
	}

	FVector Velocity = BaseCharacter->GetVelocity();
	MoveSpeed = Velocity.Size();
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, BaseCharacter->GetActorRotation());
	bMoving = !Velocity.IsNearlyZero(0.0005f);
}

void UGHBaseAnimInstance::BeginDestroy()
{
	Super::BeginDestroy();

	BaseCharacter = nullptr;
}
