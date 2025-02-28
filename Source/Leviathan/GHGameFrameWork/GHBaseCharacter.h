// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Leviathan/AbilitySystem/Component/GHAbilitySystemComponent.h"
#include "Leviathan/Animation/GHAnimationLib.h"
#include "GHBaseCharacter.generated.h"

class USkillKnockComponent;
class UGHAnimationLib;

UCLASS()
class LEVIATHAN_API AGHBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGHBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable)
	void InitAbilitySystemComponent(AActor* OwnerActor);

	UFUNCTION(BlueprintCallable)
	int32 GetID();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual FGHAnimMontageInfo* GetAnimMontageInfo(FGameplayTag GameplayTag) const;

	/**
	 * @brief ASC组件
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UGHAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Knock")
	USkillKnockComponent* SkillKnockComponent;

	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable, WithValidation)
	void NetMulticast_PlayAnimMontage(UAnimMontage* montage);
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void NetMulticast_StopAnimMontage(class UAnimMontage* AnimMontage = nullptr);

private:
	bool ASCInitialized = false;

private:
	static int32 IDFlag;

	UPROPERTY(Replicated)
	int32 ID;//唯一id，标识一个Character
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UGHAnimationLib> AnimationLib;
};
