// Fill out your copyright notice in the Description page of Project Settings.


#include "GHBaseCharacter.h"

#include "Leviathan/GHComponents/SkillKnockComponent.h"
#include "Leviathan/Log/GHLog.h"
#include "Net/UnrealNetwork.h"

int32 AGHBaseCharacter::IDFlag = 0;

// Sets default values
AGHBaseCharacter::AGHBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		IDFlag++;
		ID = IDFlag;
	}

	// Ability System Component.
	AbilitySystemComponent = CreateDefaultSubobject<UGHAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	SkillKnockComponent = CreateDefaultSubobject<USkillKnockComponent>(TEXT("SkillKnockComponent"));
	SkillKnockComponent->SetIsReplicated(true);
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

void AGHBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AGHBaseCharacter, ID, COND_InitialOnly);
}

int32 AGHBaseCharacter::GetID()
{
	return ID;
}

UAbilitySystemComponent* AGHBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

FGHAnimMontageInfo* AGHBaseCharacter::GetAnimMontageInfo(FGameplayTag GameplayTag) const
{
	if (!AnimationLib)
	{
		ensureMsgf(false, TEXT("缺少 AnimationLib配置！"));
		return nullptr;
	}

	FGHAnimMontageInfo* montage_info = AnimationLib->AnimMontages.Find(GameplayTag);
	if (!montage_info)
	{
		LOG_INFO("动画库：%s缺少GameplayTag为:%s的配置！",*AnimationLib->GetName(), *GameplayTag.ToString());
		return nullptr;
	}
	return montage_info;
}

bool AGHBaseCharacter::NetMulticast_PlayAnimMontage_Validate(UAnimMontage* montage)
{
	return montage->IsValidLowLevelFast();
}

void AGHBaseCharacter::NetMulticast_StopAnimMontage_Implementation(class UAnimMontage* AnimMontage)
{
	StopAnimMontage(AnimMontage);
}

void AGHBaseCharacter::NetMulticast_PlayAnimMontage_Implementation(UAnimMontage* montage)
{
	PlayAnimMontage(montage);
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
