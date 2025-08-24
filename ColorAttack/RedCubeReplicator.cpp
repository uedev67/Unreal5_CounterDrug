#include "RedCubeReplicator.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

ARedCubeReplicator::ARedCubeReplicator()
{
    PrimaryActorTick.bCanEverTick = false;

    // ✅ 트리거 박스 생성
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;

    TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Overlap);
    TriggerBox->SetHiddenInGame(true);  // ✅ 트리거 박스는 안 보이게

    bCanReplicate = true;
}

void ARedCubeReplicator::BeginPlay()
{
    Super::BeginPlay();

    // ✅ 오버랩 이벤트 바인딩
    if (TriggerBox)
    {
        TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ARedCubeReplicator::OnTriggerBoxOverlap);
    }
}

void ARedCubeReplicator::OnTriggerBoxOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    if (!bCanReplicate || !OtherActor) return;

    // ✅ Color.Red 태그 확인
    if (!OtherActor->ActorHasTag(FName("Color.Red")))
    {
        UE_LOG(LogTemp, Warning, TEXT("Not a red cube: %s"), *OtherActor->GetName());
        return;
    }

    // ✅ 현재 트리거 내부 큐브 개수 확인
    int32 CurrentCount = GetCurrentRedCubeCount();
    if (CurrentCount >= MaxCubesInTrigger)
    {
        UE_LOG(LogTemp, Warning, TEXT("TriggerBox full (%d/%d). Can't replicate."), CurrentCount, MaxCubesInTrigger);
        return;
    }

    // ✅ 복제 실행
    ReplicateCube(OtherActor);

    // ✅ 쿨다운 시작
    bCanReplicate = false;

	GetWorldTimerManager().SetTimer(
		ReplicationCooldownHandle,             // ✅ 멤버 변수 참조 사용
		this,
		&ARedCubeReplicator::ResetReplication, // ✅ 쿨다운 끝나면 호출할 함수
		ReplicationCooldown,                   // ✅ 3초
		false                                  // ✅ 반복 X
	);
}

void ARedCubeReplicator::ReplicateCube(AActor* OriginalCube)
{
    if (!RedCubeClass) 
    {
        UE_LOG(LogTemp, Error, TEXT("RedCubeClass not set!"));
        return;
    }

    UWorld* World = GetWorld();
    if (!World) return;

	// 큐브가 복제될 위치
    FVector BaseLocation = TriggerBox->GetComponentLocation();
    FRotator Rotation = OriginalCube->GetActorRotation();

    // ✅ 기존 큐브 포함 총 3개 → 2개만 추가 생성
    for (int32 i = 0; i < 2; i++)
    {
        FVector Offset = FVector(100.f * (i+1), 0.f, 0.f); // 간격을 두고 스폰
        FVector SpawnLocation = BaseLocation + Offset;

        AActor* NewCube = World->SpawnActor<AActor>(RedCubeClass, SpawnLocation, Rotation);
        if (NewCube)
        {
            NewCube->Tags.Add(FName("Color.Red"));
        }
    }
}

void ARedCubeReplicator::ResetReplication()
{
    bCanReplicate = true;
    UE_LOG(LogTemp, Warning, TEXT("Replicator cooldown finished. Ready again."));
}

int32 ARedCubeReplicator::GetCurrentRedCubeCount() const
{
    TArray<AActor*> OverlappingActors;
    TriggerBox->GetOverlappingActors(OverlappingActors);

    int32 RedCount = 0;
    for (AActor* Actor : OverlappingActors)
    {
        if (Actor && Actor->ActorHasTag(FName("Color.Red")))
        {
            RedCount++;
        }
    }

    return RedCount;
}
