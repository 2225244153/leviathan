// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GHBaseCharacter.h"
#include "GHBaseMonster.generated.h"

class UAIStateComponent;
class UBattleTargetComponent;
class UBehaviorTree;

UCLASS()
class LEVIATHAN_API AGHBaseMonster : public AGHBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGHBaseMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	UFUNCTION(BlueprintCallable)
	AGHBaseCharacter* GetBattleTarget();
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetState();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBattleTargetComponent* TargetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAIStateComponent* StateComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector BornLocation;
};
