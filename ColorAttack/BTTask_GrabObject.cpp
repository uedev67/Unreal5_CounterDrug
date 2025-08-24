#include "BTTask_GrabObject.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Monster1.h"

UBTTask_GrabObject::UBTTask_GrabObject()
{
    NodeName = "Grab Target Cube";
}

EBTNodeResult::Type UBTTask_GrabObject::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon) return EBTNodeResult::Failed;

    AMonster1* Monster = Cast<AMonster1>(AICon->GetPawn());
    if (!Monster) return EBTNodeResult::Failed;

    // Blackboard에서 타겟 큐브 가져오기
    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return EBTNodeResult::Failed;

    AActor* TargetCube = Cast<AActor>(BB->GetValueAsObject(GetSelectedBlackboardKey()));
    if (!TargetCube) return EBTNodeResult::Failed;

    // 몬스터가 큐브 잡기
    Monster->GrabObject(TargetCube);

    // bIsCarrying = true로 변경
    BB->SetValueAsBool(FName("bIsCarrying"), true);

    return EBTNodeResult::Succeeded;
}
