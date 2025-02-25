// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillUtils.h"

#include "Leviathan/GameDefine.h"

TMap<int32, FSkillData> USkillUtils::SkillDatas;

void USkillUtils::ClearCacheDatas()
{
	SkillDatas.Empty();
}

FSkillData USkillUtils::GetSkillDataBySkillID(int32 skillID)
{
	FSkillData* skillDataPtr = SkillDatas.Find(skillID);
	if (skillDataPtr != nullptr)
	{
		return *skillDataPtr;
	}

	UDataTable* SkillDataTablePtr = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/GH_Work/Data/Skill/DT_Skill.DT_Skill'"));
	if (!SkillDataTablePtr)
	{
		return FSkillData();
	}

	skillDataPtr = SkillDataTablePtr->FindRow<FSkillData>(FName(*FString::FromInt(skillID)), "");
	if (!skillDataPtr)
	{
		return FSkillData();
	}
	FSkillData skillData = *skillDataPtr;
	SkillDatas.Emplace(skillID, skillData);
	
	return skillData;
}
