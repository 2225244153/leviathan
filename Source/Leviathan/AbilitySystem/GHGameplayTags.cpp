

#include "GHGameplayTags.h"

FGHGameplayTags FGHGameplayTags::GameplayTags;

void FGHGameplayTags::InitNativeTags()
{
	GH_DEFINE_GAMEPLAY_TAG_COMMENT(Ability, "Ability", "技能基础Tag");
	GH_DEFINE_GAMEPLAY_TAG_COMMENT(Event, "Event", "事件Tag");
	GH_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue, "GameplayCue", "特效Tag");
	GH_DEFINE_GAMEPLAY_TAG_COMMENT(Effect, "Effect", "效果Tag");
	GH_DEFINE_GAMEPLAY_TAG_COMMENT(Character, "Character", "角色Tag");
	GH_DEFINE_GAMEPLAY_TAG_COMMENT(CoolDown, "CoolDown", "冷却Tag");
	GH_DEFINE_GAMEPLAY_TAG_COMMENT(Input, "Input", "输入");
	GH_DEFINE_GAMEPLAY_TAG_COMMENT(Anim, "Anim", "动画");
}

FGHGameplayTags::FGHGameplayTags()
{

}
