// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "GHBaseAIController.generated.h"

class AGHBaseMonster;

UCLASS()
class LEVIATHAN_API AGHBaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGHBaseAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetBornLocation(FVector& bornLocation);
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

protected:
	UPROPERTY()
	AGHBaseMonster* BaseMonster;
};
