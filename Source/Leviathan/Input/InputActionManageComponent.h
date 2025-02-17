// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GHInputAction.h"
#include "InputActionManageComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LEVIATHAN_API UInputActionManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInputActionManageComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void InitInputAction(APlayerController* InController);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InputAction")
	TObjectPtr<UGHInputActionBindingList> InputActionBindings;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	ACharacter* Character;
};
