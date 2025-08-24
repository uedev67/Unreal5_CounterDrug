#include "BTTask_DropObject.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Monster1.h"

UBTTask_DropObject::UBTTask_DropObject()
{
    NodeName = "Drop Cube";
}

EBTNodeResult::Type UBTTask_DropObject::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon) return EBTNodeResult::Failed;

    AMonster1* Monster = Cast<AMonster1>(AICon->GetPawn());
    if (!Monster) return EBTNodeResult::Failed;

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return EBTNodeResult::Failed;

    // 몬스터가 큐브 놓기
    Monster->DropObject();

    // bIsCarrying = false로 변경
    BB->SetValueAsBool(FName("bIsCarrying"), false);

    return EBTNodeResult::Succeeded;
}
