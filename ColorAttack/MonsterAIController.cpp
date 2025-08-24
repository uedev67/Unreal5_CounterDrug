#include "MonsterAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AMonsterAIController::AMonsterAIController()
{
    // Behavior Tree는 BeginPlay에서 실행
}

void AMonsterAIController::BeginPlay()
{
    Super::BeginPlay();

    if (BehaviorTreeAsset)
    {
        UE_LOG(LogTemp, Warning, TEXT("MonsterAIController: Running BT"));
        RunBehaviorTree(BehaviorTreeAsset);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("MonsterAIController: BehaviorTreeAsset is NULL!"));
    }
}
