#include "GHCoreDelegatesMgr.h"

TMulticastDelegate<void(AGHBaseCharacter*)> UGHCoreDelegatesMgr::OnBattleFindTarget;
TMulticastDelegate<void()> UGHCoreDelegatesMgr::OnBattleLoseTarget;
TDelegate<void()> UGHCoreDelegatesMgr::OnStartAlert;
TDelegate<void()> UGHCoreDelegatesMgr::OnFinishAlert;

void UGHCoreDelegatesMgr::ClearDelegates()
{
	OnBattleFindTarget.Clear();
	OnBattleLoseTarget.Clear();
	OnStartAlert.Unbind();
	OnFinishAlert.Unbind();
}
