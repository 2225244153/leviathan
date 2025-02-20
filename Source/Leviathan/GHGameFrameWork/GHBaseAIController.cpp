// Fill out your copyright notice in the Description page of Project Settings.


#include "GHBaseAIController.h"

#include "GHBaseMonster.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AGHBaseAIController::AGHBaseAIController() : BaseMonster(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGHBaseAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGHBaseAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AGHBaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGHBaseAIController::SetBornLocation(FVector& bornLocation)
{
	if (Blackboard != nullptr)
	{
		Blackboard->SetValueAsVector(TEXT("BornLocation"), bornLocation);
	}
}

void AGHBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	BaseMonster = Cast<AGHBaseMonster>(InPawn);
	if (BaseMonster == nullptr || BaseMonster->BehaviorTreeAsset == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GH---Monter BehaivourTree is NULL!!!"));
		return;
	}
	
	RunBehaviorTree(BaseMonster->BehaviorTreeAsset);
}

void AGHBaseAIController::OnUnPossess()
{
	Super::OnUnPossess();

	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComp != nullptr)
	{
		BTComp->StopTree();
	}
	BaseMonster = nullptr;
}

