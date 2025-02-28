
#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "GHTargetActor.generated.h"

class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTagChangeDelegate, FGameplayTag, NewTag);

// Target Actor是由技能释放出 为技能的释放提供可视化效果 为玩家提供技能释放所需信息的Actor
// 所有与旋转相关的信息从技能处获取 并为技能的释放提供位置 目标等信息

UCLASS(Blueprintable, BlueprintType)
class LEVIATHAN_API AGHTargetActor : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGHTargetActor();

public:
	virtual void ConfirmTargeting() override;
	virtual void CancelTargeting() override;
	virtual void ConfirmTargetingAndContinue() override;
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (DisplayName = "ConfirmTargeting"))
	void K2_ConfirmTargeting();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (DisplayName = "CancelTargeting"))
	void K2_CancelTargeting();

	USphereComponent* GetSphereCollision();

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "RootPoint")
	USceneComponent *RootPoint;
	
	UPROPERTY(BlueprintAssignable, Category = "TargetActorState")
	FOnTagChangeDelegate OnTagChangeDelegate;
};

