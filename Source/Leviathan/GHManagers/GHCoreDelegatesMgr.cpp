#include "GHCoreDelegatesMgr.h"

UGHCoreDelegatesMgr::UGHCoreDelegatesMgr()
{
}

void UGHCoreDelegatesMgr::ClearDelegates()
{
	OnBattleSearchTarget.Unbind();
	OnBattleLoseTarget.Unbind();
	OnStartAlert.Unbind();
	OnFinishAlert.Unbind();
	OnAIStateChanged.Clear();
	OnCharacterHurt.Clear();
	OnCharacterDeath.Clear();
	OnCharacterEndAbility.Clear();
	OnCharacterEndAbility.Clear();
}
