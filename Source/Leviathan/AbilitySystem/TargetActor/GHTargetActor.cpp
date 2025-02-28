#include "GHTargetActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerController.h"

AGHTargetActor::AGHTargetActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootPoint = CreateDefaultSubobject<USceneComponent>("RootPoint");
	SetRootComponent(RootPoint);
}

void AGHTargetActor::ConfirmTargeting()
{
	Super::ConfirmTargeting();
	K2_ConfirmTargeting();
}

void AGHTargetActor::CancelTargeting()
{
	Super::CancelTargeting();
	K2_CancelTargeting();
}

void AGHTargetActor::ConfirmTargetingAndContinue()
{
	USphereComponent* Sphere = GetSphereCollision();
	TArray<AActor*> overlapped_actors;
	Sphere->GetOverlappingActors(overlapped_actors);
	FGameplayAbilityTargetDataHandle Handle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActorArray(
		overlapped_actors, true);

	TargetDataReadyDelegate.Broadcast(Handle);
}

USphereComponent* AGHTargetActor::GetSphereCollision()
{
	USphereComponent* Sphere = Cast<USphereComponent>(GetComponentByClass(USphereComponent::StaticClass()));
	return Sphere;
}