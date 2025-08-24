#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

UCLASS()
class COLORATTACK_API AMonsterAIController : public AAIController
{
    GENERATED_BODY()

public:
    AMonsterAIController();
    virtual void BeginPlay() override;

protected:
    // Behavior Tree 실행만 남김
    UPROPERTY(EditDefaultsOnly, Category="AI")
    UBehaviorTree* BehaviorTreeAsset;
};
