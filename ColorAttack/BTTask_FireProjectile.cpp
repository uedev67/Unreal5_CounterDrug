// BTTask_FireProjectile.cpp
#include "BTTask_FireProjectile.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster2.h"
#include "Kismet/KismetMathLibrary.h"
#include "Async/Async.h" 
#include "GameFramework/ProjectileMovementComponent.h"



UBTTask_FireProjectile::UBTTask_FireProjectile()
{
    NodeName = "Fire Projectile At Player";
    bNotifyTaskFinished = true;
}


EBTNodeResult::Type UBTTask_FireProjectile::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    auto* AICon = OwnerComp.GetAIOwner();
    ControlledMonster = Cast<AMonster2>(AICon->GetPawn());
    if (!ControlledMonster) return EBTNodeResult::Failed;

    auto* BB = OwnerComp.GetBlackboardComponent();
    CachedPlayer = Cast<AActor>(BB->GetValueAsObject("PlayerActor"));
    if (!CachedPlayer) return EBTNodeResult::Failed;

    CachedOwnerComp = &OwnerComp;
    ShotsFired = 0;

    // 먼저 HandleFire 호출 → 내부에서 스턴 체크 후 반복 여부 결정
    HandleFire();

    return EBTNodeResult::InProgress;

}



void UBTTask_FireProjectile::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

    if (ControlledMonster)
    {
        ControlledMonster->GetWorldTimerManager().ClearTimer(FireTimerHandle);
    }

    ControlledMonster = nullptr;
    CachedPlayer = nullptr;
    CachedOwnerComp = nullptr;
    ShotsFired = 0;

}


void UBTTask_FireProjectile::HandleFire()
{
    if (!ControlledMonster || !CachedPlayer || !CachedOwnerComp) return;

    UBlackboardComponent* BB = ControlledMonster->GetController() ?
        Cast<AAIController>(ControlledMonster->GetController())->GetBlackboardComponent() : nullptr;

    if (!BB)
    {
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Failed);
        return;
    }

    if (BB->GetValueAsBool(FName("bIsStunned")))
    {
        ControlledMonster->GetWorldTimerManager().SetTimer(
            FireTimerHandle, this, &UBTTask_FireProjectile::HandleFire, 0.5f, false
        );
        return;
    }

    float Distance = FVector::Dist(ControlledMonster->GetActorLocation(), CachedPlayer->GetActorLocation());
    if (Distance > FireRange)
    {
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    // 발사
    FVector MuzzleLoc = ControlledMonster->GetActorLocation() + FVector(0, 0, 80);
    FRotator MuzzleRot = UKismetMathLibrary::FindLookAtRotation(MuzzleLoc, CachedPlayer->GetActorLocation());

    AActor* Projectile = ControlledMonster->GetWorld()->SpawnActor<AActor>(
        ControlledMonster->ProjectileClass, MuzzleLoc, MuzzleRot
    );

    if (Projectile)
    {
        if (auto* ProjMove = Projectile->FindComponentByClass<UProjectileMovementComponent>())
            ProjMove->Velocity = MuzzleRot.Vector() * ProjMove->InitialSpeed;

        Projectile->SetLifeSpan(6.0f);
    }

    ShotsFired++;
    if (ShotsFired >= MaxShots)
    {
        UE_LOG(LogTemp, Warning, TEXT("FireProjectile: All shots fired"));
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
    else
    {
        ControlledMonster->GetWorldTimerManager().SetTimer(
            FireTimerHandle, this, &UBTTask_FireProjectile::HandleFire, FireInterval, false
        );
    }
}



