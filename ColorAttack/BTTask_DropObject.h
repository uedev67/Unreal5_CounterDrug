#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_DropObject.generated.h"

UCLASS()
class COLORATTACK_API UBTTask_DropObject : public UBTTask_BlackboardBase
{
    GENERATED_BODY()

public:
    UBTTask_DropObject();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
