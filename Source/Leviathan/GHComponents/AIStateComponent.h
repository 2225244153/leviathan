#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "Components/ActorComponent.h"
#include "AIStateComponent.generated.h"

class AGHBaseCharacter;
class AGHBaseMonster;

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/*
	 * CoreDelegates begin
	 */
	void OnBattleSearchTarget(int32 targetId);
	void OnBattleLoseTarget(uint8 loseType);
	void OnStartAlert();
	void OnFinishAlert();
	UFUNCTION()
	void OnDeath(int32 sponsorId, int32 targetId);
	UFUNCTION()
	void OnHurt(int32 sponsorId, int32 targetId);
	/*
	 * CoreDelegates end
	 */

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetState();
	UFUNCTION(BlueprintCallable)
	void SetState(FGameplayTag state);

	/*
	 *	寻找状态
	 */
	void StartFinding();
	void FinishFinding();
	void UpdateFindingTime(float DeltaTime);

	//检测是否回到出生点
	void CheckBackBornLocation(float DeltaTime);

public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "AIStateParams")
	FGameplayTag TagState;
	//最大寻找时间，超过此寻找时间返回出生点
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIStateParams")
	float MaxFindingTime = 10.f;

private:
	UPROPERTY()
	AGHBaseMonster* Owner;
	bool bFinding;
	//当前寻找时间
	float CurFindingTime;
	float CurBackTime;
};

