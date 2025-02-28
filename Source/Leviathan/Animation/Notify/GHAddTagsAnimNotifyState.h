#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "UObject/Object.h"
#include "GHAddTagsAnimNotifyState.generated.h"

/**
 * 添加Tags ANS.
 */
UCLASS(Blueprintable, BlueprintType)
class LEVIATHAN_API UGHAddTagsAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override;
#if WITH_EDITOR

	// Overridden from UAnimNotifyState to provide custom notify name.
	virtual FString GetNotifyName_Implementation() const override;
#endif

public:
	/**
	 * @brief 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tags")
	FGameplayTagContainer AddTagContainer;

	/**
	 * @brief 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tags")
	bool ReplicateTags = false;
};
