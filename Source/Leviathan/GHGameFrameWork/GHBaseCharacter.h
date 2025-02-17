// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Leviathan/AbilitySystem/Component/GHAbilitySystemComponent.h"
#include "GHBaseCharacter.generated.h"

UCLASS()
class LEVIATHAN_API AGHBaseCharacter : public ACharacter,public IAbilitySystemInterface
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

public:
	UFUNCTION(BlueprintCallable)
	void InitAbilitySystemComponent(AActor* OwnerActor);

	UFUNCTION(BlueprintCallable)
	int32 GetID();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	/**
	 * @brief ASC组件
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UGHAbilitySystemComponent* AbilitySystemComponent;

private:
	bool ASCInitialized = false;

private:
	static int32 IDFlag;
	int32 ID; //唯一id，标识一个Character
};
