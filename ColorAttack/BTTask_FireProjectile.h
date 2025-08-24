// BTTask_FireProjectile.h
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TimerManager.h"
#include "BTTask_FireProjectile.generated.h"

UCLASS()
class COLORATTACK_API UBTTask_FireProjectile : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_FireProjectile();

    /** 발사 간격 */
    UPROPERTY(EditAnywhere, Category="Attack")
    float FireInterval = 1.0f;

    /** 몇 발 쏠지 */
    UPROPERTY(EditAnywhere, Category="Attack")
    int32 MaxShots = 2;

    /** 사거리 */
    UPROPERTY(EditAnywhere, Category="Attack")
    float FireRange = 1300.f;

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
    int32 ShotsFired;
    float TimeSinceLastShot;
    AActor* CachedPlayer;
    class AMonster2* ControlledMonster;

    FTimerHandle FireTimerHandle;
    UBehaviorTreeComponent* CachedOwnerComp;

    void HandleFire(); //  타이머 콜백 함수
};
