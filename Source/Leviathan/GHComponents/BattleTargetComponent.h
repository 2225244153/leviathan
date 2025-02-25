// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "BattleTargetComponent.generated.h"

struct FGameplayTag;
class AGHBaseMonster;
class AGHBaseCharacter;

enum ELoseTargetType : uint8
{
	E_LoseTargetType_Normal = 0,	//正常超过追击范围丢失目标
	E_LoseTargetType_Back = 1,		//超过出生点范围，返回出生点丢失目标
};

/*
 * 查找战斗目标组件，默认该组件只绑定BaseMonster，只会查找BasePlayer为目标
 * 角色查找怪物为攻击目标不支持 todo
 * 暂时不支持切换目标的逻辑
 * @company GH
 * @author hsp
 * @date 2025-2-14
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LEVIATHAN_API UBattleTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBattleTargetComponent();

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
	UFUNCTION()
	void OnAIStateChanged(FGameplayTag& oldTag, FGameplayTag& newTag);
	UFUNCTION()
	void OnHurt(int32 sponsorId, int32 targetId);
	/*
	 * CoreDelegates end
	 */

	UFUNCTION(BlueprintCallable)
	AGHBaseCharacter* GetBattleTarget();
	UFUNCTION(BlueprintCallable)
	void SetBattleTarget(AGHBaseCharacter* target);
	
	void LoseBattleTarget(ELoseTargetType loseType);

	/*
	 *	寻找目标
	 */
	void SearchBattleTarget();
	/*
	 * 判断目标是否合法 目标为空或者超过范围则为不合法
	 */
	bool CheckTargetValid();

	/*
	 *	警戒
	 */
	void StartAlert();
	void FinishAlert();
	void UpdateAlert(float DeltaTime);
	void SetAlertTarget(AGHBaseCharacter* alertTarget);
	
	/*
	 *	检查返回距离
	 */
	void CheckBackDistance();
	
public:
	//警戒最大值，超过最大值则设置目标进入战斗状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SearchTargetParams")
	int32 MaxAlertValue = 100;
	//警戒值每秒浮动速率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SearchTargetParams")
	float AlertFloatRatePerSecond = 20.f;
	//丢失目标范围，超过此范围丢失目标
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SearchTargetParams")
	float LoseTargetDistance = 1500.f;
	//发现目标的角度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SearchTargetParams")
	float SearchTargetAngle = 60.f;
	//搜寻目标警戒最大范围
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SearchTargetParams")
	float SearchTargetWarnMaxDistance = 1000.f;
	//返回出生点距离，超过此距离直接丢失目标返回出生点
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SearchTargetParams")
	float BackDistance = 3000.f;
	
private:
	UPROPERTY(Replicated)
	AGHBaseCharacter* BattleTarget;
	UPROPERTY(Replicated)
	AGHBaseCharacter* AlertTarget;
	UPROPERTY()
	AGHBaseMonster* Owner;
	//寻找目标开关
	bool bSearchBattleTarget = true;
	//警戒状态开关
	bool bSearchAlertTarget = true;
	//是否在警戒状态
	bool bAlert = false;
	//当前警戒值
	float CurAlertValue = 0.f;
};
