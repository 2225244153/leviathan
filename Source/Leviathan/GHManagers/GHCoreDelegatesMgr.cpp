#include "GHCoreDelegatesMgr.h"

TMulticastDelegate<void(AGHBaseCharacter*)> UGHCoreDelegatesMgr::OnBattleSearchTarget;
TMulticastDelegate<void(uint8)> UGHCoreDelegatesMgr::OnBattleLoseTarget;
TDelegate<void()> UGHCoreDelegatesMgr::OnStartAlert;
TDelegate<void()> UGHCoreDelegatesMgr::OnFinishAlert;
TMulticastDelegate<void(FGameplayTag&, FGameplayTag&)> UGHCoreDelegatesMgr::OnAIStateChanged;

void UGHCoreDelegatesMgr::ClearDelegates()
{
	OnBattleSearchTarget.Clear();
	OnBattleLoseTarget.Clear();
	OnStartAlert.Unbind();
	OnFinishAlert.Unbind();
	OnAIStateChanged.Clear();
}
