#include "GHCoreDelegatesMgr.h"

TMulticastDelegate<void(int32)> UGHCoreDelegatesMgr::OnBattleSearchTarget;
TMulticastDelegate<void(uint8)> UGHCoreDelegatesMgr::OnBattleLoseTarget;
TDelegate<void()> UGHCoreDelegatesMgr::OnStartAlert;
TDelegate<void()> UGHCoreDelegatesMgr::OnFinishAlert;
TMulticastDelegate<void(FGameplayTag&, FGameplayTag&)> UGHCoreDelegatesMgr::OnAIStateChanged;
TMulticastDelegate<void(int32, int32)> UGHCoreDelegatesMgr::OnCharacterHurt;
TMulticastDelegate<void(int32, int32)> UGHCoreDelegatesMgr::OnCharacterDeath;

void UGHCoreDelegatesMgr::ClearDelegates()
{
	OnBattleSearchTarget.Clear();
	OnBattleLoseTarget.Clear();
	OnStartAlert.Unbind();
	OnFinishAlert.Unbind();
	OnAIStateChanged.Clear();
	OnCharacterHurt.Clear();
	OnCharacterDeath.Clear();
}
