// Fill out your copyright notice in the Description page of Project Settings.


#include "GHBaseMonster.h"

#include "GHBaseAIController.h"
#include "Leviathan/GHComponents/AIStateComponent.h"
#include "Leviathan/GHComponents/BattleTargetComponent.h"


// Sets default values
AGHBaseMonster::AGHBaseMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (HasAnyFlags(RF_ClassDefaultObject) && GetClass() == AGHBaseMonster::StaticClass())
	{
		FString AIControllerClassName = GetDefault<UEngine>()->AIControllerClassName.ToString();
		check(FPackageName::IsValidObjectPath(AIControllerClassName));
		AIControllerClass = FindObject<UClass>(nullptr, *AIControllerClassName);
		if (AIControllerClass == nullptr)
		{
			AIControllerClass = LoadClass<AGHBaseAIController>(nullptr, *AIControllerClassName, nullptr, LOAD_None, nullptr);
		}
	}
	else
	{
		AIControllerClass = ((AGHBaseMonster*)AGHBaseMonster::StaticClass()->GetDefaultObject())->AIControllerClass;
	}

	TargetComponent = CreateDefaultSubobject<UBattleTargetComponent>(TEXT("TargetComponent"));
	StateComponent = CreateDefaultSubobject<UAIStateComponent>(TEXT("StateComponent"));
	TargetComponent->SetIsReplicated(true);
	StateComponent->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void AGHBaseMonster::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();

	BornLocation = GetActorLocation();
	AGHBaseAIController* aiController = Cast<AGHBaseAIController>(GetController());
	if (aiController != nullptr)
	{
		aiController->SetBornLocation(BornLocation);
	}
}

// Called every frame
void AGHBaseMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGHBaseMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

AGHBaseCharacter* AGHBaseMonster::GetBattleTarget()
{
	return TargetComponent->GetBattleTarget();
}

FGameplayTag AGHBaseMonster::GetState()
{
	return StateComponent->GetState();
}

