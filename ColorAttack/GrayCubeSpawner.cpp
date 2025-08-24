// GrayCubeSpawner.cpp
#include "GrayCubeSpawner.h"
#include "GrayCube.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"     // ★ 추가
#include "UObject/ConstructorHelpers.h"

AGrayCubeSpawner::AGrayCubeSpawner()
{
    PrimaryActorTick.bCanEverTick = true;

    /* (1) 외형용 큐브 */
    SpawnerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnerMesh"));
    RootComponent = SpawnerMesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMeshAsset.Succeeded())
    {
        SpawnerMesh->SetStaticMesh(CubeMeshAsset.Object);
        SpawnerMesh->SetWorldScale3D(FVector(3.f));
        SpawnerMesh->SetMobility(EComponentMobility::Static);
    }

    /* (2) 스폰 방향 화살표 */
    SpawnArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnArrow"));
    SpawnArrow->SetupAttachment(RootComponent);
    SpawnArrow->SetRelativeLocation(FVector(150.f, 0.f, 0.f));
    SpawnArrow->ArrowSize      = 2.f;
    SpawnArrow->bHiddenInGame  = true;     // 에디터에서만 표시
}

void AGrayCubeSpawner::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(
        SpawnTimerHandle, this,
        &AGrayCubeSpawner::SpawnGrayCube,
        SpawnInterval, true
    );
}

void AGrayCubeSpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    /* 파괴된 큐브 정리 */
    ActiveCubes.RemoveAll([](AActor* Cube)
    {
        return !IsValid(Cube) || Cube->IsActorBeingDestroyed();
    });
}

void AGrayCubeSpawner::SpawnGrayCube()
{
    if (!GrayCubeClass) return;
    if (ActiveCubes.Num() >= MaxActiveCubes) return;

    /* Arrow 기준 위치·회전 계산 */
    const FVector  SpawnLoc = SpawnArrow->GetComponentLocation()
                            + SpawnArrow->GetForwardVector() * SpawnDistance;
    const FRotator SpawnRot = SpawnArrow->GetComponentRotation();

    if (AGrayCube* NewCube = GetWorld()->SpawnActor<AGrayCube>(GrayCubeClass, SpawnLoc, SpawnRot))
    {
        ActiveCubes.Add(NewCube);
    }
}
