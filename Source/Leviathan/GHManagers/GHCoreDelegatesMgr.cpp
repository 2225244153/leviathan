#include "GHCoreDelegatesMgr.h"

#include "Leviathan/GHGameFrameWork/GHGameInstace.h"

UGHGameInstace* UGHCoreDelegatesMgr::Inst = nullptr;

UGHCoreDelegatesMgr::UGHCoreDelegatesMgr()
{
}

UGHCoreDelegatesMgr* UGHCoreDelegatesMgr::Get()
{
	return UGameInstance::GetSubsystem<UGHCoreDelegatesMgr>(Inst);
}

void UGHCoreDelegatesMgr::GHInit(UGHGameInstace* inst)
{
	Inst = inst;
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
