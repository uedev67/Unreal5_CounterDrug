#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GrabObject.generated.h"

UCLASS()
class COLORATTACK_API UBTTask_GrabObject : public UBTTask_BlackboardBase
{
    GENERATED_BODY()

public:
    UBTTask_GrabObject();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
