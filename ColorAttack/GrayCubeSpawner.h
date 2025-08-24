// GrayCubeSpawner.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrayCubeSpawner.generated.h"

class AGrayCube;
class UStaticMeshComponent;
class UArrowComponent;            // ★ 추가

UCLASS()
class COLORATTACK_API AGrayCubeSpawner : public AActor
{
    GENERATED_BODY()

public:
    AGrayCubeSpawner();

protected:
    virtual void BeginPlay() override;
    UFUNCTION()
    void SpawnGrayCube();

public:
    virtual void Tick(float DeltaTime) override;

/* ===== 스폰 설정 ===== */
    /** 몇 초마다 생성? */
    UPROPERTY(EditAnywhere, Category="Spawner")
    float SpawnInterval = 4.f;

    /** 동시에 유지할 최대 큐브 수 */
    UPROPERTY(EditAnywhere, Category="Spawner")
    int32 MaxActiveCubes = 2;

    /** 스폰할 큐브 BP 클래스 지정 */
    UPROPERTY(EditAnywhere, Category="Spawner")
    TSubclassOf<AGrayCube> GrayCubeClass;

    /** Arrow 앞쪽으로 띄울 거리(cm) */
    UPROPERTY(EditAnywhere, Category="Spawner")
    float SpawnDistance = 300.f;

/* ===== 컴포넌트 ===== */
    /** 공장 외형 */
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* SpawnerMesh;

    /** 스폰 방향 표시용 화살표 */
    UPROPERTY(VisibleAnywhere)
    UArrowComponent* SpawnArrow;

private:
    TArray<AActor*> ActiveCubes;
    FTimerHandle    SpawnTimerHandle;
};
