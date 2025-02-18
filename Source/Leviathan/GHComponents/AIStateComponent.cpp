
#include "AIStateComponent.h"

#include "Leviathan/GHManagers/GHCoreDelegatesMgr.h"

UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Init, "AI.State.Init");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Alert, "AI.State.Alert");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Battle, "AI.State.Battle");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Find, "AI.State.Find");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Back, "AI.State.Back");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Death, "AI.State.Death");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Battle_Pursue, "AI.State.Battle.Pursue");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Battle_Turn, "AI.State.Battle.Turn");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Battle_Attack, "AI.State.Battle.Attack");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Battle_Perform, "AI.State.Battle.Perform");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Battle_Hurt, "AI.State.Battle.Hurt");

// Sets default values for this component's properties
UAIStateComponent::UAIStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAIStateComponent::BeginPlay()
{
	Super::BeginPlay();

	SetState(AI_Monster_State_Init);

	UGHCoreDelegatesMgr::OnBattleFindTarget.AddUObject(this, &UAIStateComponent::OnBattleFindTarget);
	UGHCoreDelegatesMgr::OnBattleLoseTarget.AddUObject(this, &UAIStateComponent::OnBattleLoseTarget);
	UGHCoreDelegatesMgr::OnStartAlert.BindUObject(this, &UAIStateComponent::OnStartAlert);
	UGHCoreDelegatesMgr::OnFinishAlert.BindUObject(this, &UAIStateComponent::OnFinishAlert);
}

// Called every frame
void UAIStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAIStateComponent::OnBattleFindTarget(AGHBaseCharacter* target)
{
	SetState(AI_Monster_State_Battle);
}

void UAIStateComponent::OnBattleLoseTarget()
{
	SetState(AI_Monster_State_Find);
}

void UAIStateComponent::OnStartAlert()
{
	SetState(AI_Monster_State_Alert);
}

void UAIStateComponent::OnFinishAlert()
{
	SetState(AI_Monster_State_Init);
}

FGameplayTag UAIStateComponent::GetState()
{
	return TagState;
}

void UAIStateComponent::SetState(FGameplayTag state)
{
	UE_LOG(LogTemp, Warning, TEXT("GH------UAIStateComponent::SetState: %s"), *state.ToString());
	TagState = state;
}

