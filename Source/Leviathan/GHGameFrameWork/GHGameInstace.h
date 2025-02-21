// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GHGameInstace.generated.h"

/**
 * 
 */
UCLASS()
class LEVIATHAN_API UGHGameInstace : public UGameInstance
{
	GENERATED_BODY()

public:
	UGHGameInstace();
	~UGHGameInstace();

	void GHInit();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=GHGameInstace)
	class UGHCharacterMgr* CharacterMgr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=GHGameInstace)
	class UGHCoreDelegatesMgr* CoreDelegatesMgr;
	
protected:
	virtual void StartGameInstance() override;
	virtual void Shutdown() override;

#if WITH_EDITOR
	virtual FGameInstancePIEResult StartPlayInEditorGameInstance(ULocalPlayer* LocalPlayer, const FGameInstancePIEParameters& Params) override;
#endif
};
