// Fill out your copyright notice in the Description page of Project Settings.


#include "GHBaseCharacter.h"

int32 AGHBaseCharacter::ID = 0;

// Sets default values
AGHBaseCharacter::AGHBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		ID++;
	}

	// Ability System Component.
	AbilitySystemComponent = CreateDefaultSubobject<UGHAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

// Called when the game starts or when spawned
void AGHBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	ENetRole LocalRole = GetLocalRole();
	if (LocalRole >= ROLE_AutonomousProxy)
	{
		InitAbilitySystemComponent(this);
	}
}

// Called every frame
void AGHBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int32 AGHBaseCharacter::GetID()
{
	return ID;
}

UAbilitySystemComponent* AGHBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGHBaseCharacter::InitAbilitySystemComponent(AActor* OwnerActor)
{
	// 防止重复初始化
	if (ASCInitialized)
	{
		return;
	}
	AbilitySystemComponent->InitAbilityActorInfo(OwnerActor, this);

	ENetRole LocalRole = GetLocalRole();
	if (LocalRole == ROLE_Authority)
	{
		AbilitySystemComponent->InitAttributeSet();
		AbilitySystemComponent->InitAbility();
	}
	
	ASCInitialized = true;
}
