#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "Components/ActorComponent.h"
#include "AIStateComponent.generated.h"

class AGHBaseCharacter;
UE_DECLARE_GAMEPLAY_TAG_EXTERN(AI_Monster_State_Init)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(AI_Monster_State_Alert)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(AI_Monster_State_Battle)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(AI_Monster_State_Find)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(AI_Monster_State_Back)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(AI_Monster_State_Death)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(AI_Monster_State_Battle_Pursue)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(AI_Monster_State_Battle_Turn)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(AI_Monster_State_Battle_Attack)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(AI_Monster_State_Battle_Perform)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(AI_Monster_State_Battle_Hurt)

/*
 * AI状态管理组件，负责状态的切换
 * @company GH
 * @author hsp
 * @date 2025-2-17
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LEVIATHAN_API UAIStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAIStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*
	 * CoreDelegates begin
	 */
	void OnBattleFindTarget(AGHBaseCharacter* target);
	void OnBattleLoseTarget();
	void OnStartAlert();
	void OnFinishAlert();
	/*
	 * CoreDelegates end
	 */

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetState();
	UFUNCTION(BlueprintCallable)
	void SetState(FGameplayTag state);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=AIState)
	FGameplayTag TagState;
};
