
#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

class UGameplayTagsManager;

#define GH_DEFINE_GAMEPLAY_TAG_COMMENT(TagName, Tag, Comment) \
TagName = UGameplayTagsManager::Get().AddNativeGameplayTag(Tag, Comment)

/**
 * @brief 
 */
struct LEVIATHAN_API FGHGameplayTags
{
	FGameplayTag Ability;
	FGameplayTag Event;
	FGameplayTag GameplayCue;
	FGameplayTag Effect;
	FGameplayTag Character;
	FGameplayTag CoolDown;
	FGameplayTag Input;
	FGameplayTag Anim;

	void InitNativeTags();

	FGHGameplayTags();

	static FGHGameplayTags& Get() { return GameplayTags; }
	static FGHGameplayTags GameplayTags;
};
