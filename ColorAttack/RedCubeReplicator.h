#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RedCubeReplicator.generated.h"

class UBoxComponent;

UCLASS()
class COLORATTACK_API ARedCubeReplicator : public AActor
{
    GENERATED_BODY()

public:
    ARedCubeReplicator();

protected:
    virtual void BeginPlay() override;

    /** ✅ 트리거 박스 */
    UPROPERTY(VisibleAnywhere, Category="Replicator")
    UBoxComponent* TriggerBox;

    /** ✅ Cooldown 상태 */
    bool bCanReplicate;

    /** ✅ 트리거 박스 내부 최대 큐브 개수 */
    UPROPERTY(EditAnywhere, Category="Replicator")
    int32 MaxCubesInTrigger = 6;

    /** ✅ 복제 대기 시간 (3초) */
    UPROPERTY(EditAnywhere, Category="Replicator")
    float ReplicationCooldown = 3.0f;

	/** ✅ 복제 쿨다운 타이머 핸들 */
    FTimerHandle ReplicationCooldownHandle;

    /** ✅ 큐브 클래스 (스폰할 대상) */
    UPROPERTY(EditAnywhere, Category="Replicator")
    TSubclassOf<AActor> RedCubeClass;

    /** ✅ 오버랩 이벤트 */
    UFUNCTION()
    void OnTriggerBoxOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    /** ✅ 실제 복제 실행 */
    void ReplicateCube(AActor* OriginalCube);

    /** ✅ 쿨다운 리셋 */
    void ResetReplication();

    /** ✅ 현재 트리거 내부 레드큐브 개수 */
    int32 GetCurrentRedCubeCount() const;
};
