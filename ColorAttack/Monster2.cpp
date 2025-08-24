#include "Monster2.h" 
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ColorAttackGameMode.h" 
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"


AMonster2::AMonster2()
{
    PrimaryActorTick.bCanEverTick = true;

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    StunZone = CreateDefaultSubobject<UBoxComponent>(TEXT("StunZone"));
    StunZone->SetupAttachment(RootComponent);
    StunZone->SetBoxExtent(FVector(100.f, 100.f, 80.f)); // 머리만 감지
    StunZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    StunZone->SetCollisionObjectType(ECC_WorldDynamic);
    StunZone->SetCollisionResponseToAllChannels(ECR_Ignore);
    StunZone->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
}

void AMonster2::BeginPlay()
{
    Super::BeginPlay();

    if (StunZone)
    {
        StunZone->OnComponentBeginOverlap.AddDynamic(this, &AMonster2::OnHeadOverlap);  // 머리 맞으면 스턴 걸리기
    }
}

void AMonster2::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Player) return;

}



void AMonster2::OnHeadOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!OtherActor || bIsStunned) return;
  
    if (OtherActor->ActorHasTag(FName("Color.Red")))   //  빨간 큐브가 닿으면
    {
        bIsStunned = true;

        GetCharacterMovement()->DisableMovement(); // 몬스터 정지

        // Blackboard에 bIsStunned = true 설정
        if (AAIController* AI = Cast<AAIController>(GetController()))
        {
            if (UBlackboardComponent* BB = AI->GetBlackboardComponent()) BB->SetValueAsBool(FName("bIsStunned"), true);
        }

        if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
            {
                if (StunMontage && !Anim->Montage_IsPlaying(StunMontage)) Anim->Montage_Play(StunMontage);
            }

        GetWorldTimerManager().SetTimer(StunTimerHandle, this, &AMonster2::EndStun, 4.0f, false); // 4초동안 적용    
    }
    
    if(bIsStunned)
    {
        OtherActor->Destroy();
    }
}


void AMonster2::EndStun()
{
    bIsStunned = false;
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);    // 다시 이동 가능

    // Blackboard에 bIsStunned = false로 해제
    if (AAIController* AI = Cast<AAIController>(GetController()))
    {
        if (UBlackboardComponent* BB = AI->GetBlackboardComponent())
        {
            BB->SetValueAsBool(FName("bIsStunned"), false);
        }
    }
}