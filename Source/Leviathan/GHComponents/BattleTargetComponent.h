// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BattleTargetComponent.generated.h"

class AGHBaseMonster;
class AGHBaseCharacter;

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

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	AGHBaseCharacter* GetBattleTarget();
	UFUNCTION(BlueprintCallable)
	void SetBattleTarget(AGHBaseCharacter* target);
	//todo监听受击直接设置攻击者为目标
	
	UFUNCTION(BlueprintCallable)
	void ResetBattleTarget();

	/*
	 *	寻找目标
	 */
	void FindBattleTarget();
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
	//寻找目标开关
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FindTargetParams")
	bool bFindBattleTarget;
	//警戒最大值，超过最大值则设置目标进入战斗状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FindTargetParams")
	int32 MaxAlertValue = 100;
	//警戒值每秒浮动速率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FindTargetParams")
	float AlertFloatRatePerSecond = 20.f;
	//丢失目标范围，超过此范围丢失目标
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FindTargetParams")
	float LoseTargetDistance = 1500.f;
	//发现目标的角度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FindTargetParams")
	float FindTargetAngle = 90.f;
	//搜寻目标警戒最大范围
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FindTargetParams")
	float FindTargetWarnMaxDistance = 1000.f;
	//返回出生点距离，超过此距离直接丢失目标返回出生点
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FindTargetParams")
	float BackDistance = 3000.f;
	
private:
	UPROPERTY()
	AGHBaseCharacter* BattleTarget;
	UPROPERTY()
	AGHBaseCharacter* AlertTarget;
	UPROPERTY()
	AGHBaseMonster* Owner;

	//是否再警戒状态
	bool bAlert;
	//当前警戒值
	float CurAlertValue;
};
