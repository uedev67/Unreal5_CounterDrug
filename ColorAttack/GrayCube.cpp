#include "GrayCube.h"
#include "UObject/ConstructorHelpers.h"

AGrayCube::AGrayCube()
{
    // ✅ Root 생성
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // ✅ 메쉬 생성 (멤버 변수)
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrayMesh"));
    MeshComp->SetupAttachment(RootComponent);

    // ✅ 기본 큐브 메쉬 지정
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(
        TEXT("/Engine/BasicShapes/Cube.Cube")
    );
    if (CubeMeshAsset.Succeeded())
    {
        MeshComp->SetStaticMesh(CubeMeshAsset.Object);
    }

    // ✅ 물리 & 충돌 설정
    MeshComp->SetMobility(EComponentMobility::Movable);
    MeshComp->SetSimulatePhysics(true);
    MeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
    MeshComp->SetGenerateOverlapEvents(true);

    // ✅ 네비게이션 영향 끄기 (1차 보호)
    MeshComp->SetCanEverAffectNavigation(false);

    // ✅ 태그 부여 (회색 오브젝트 구분)
    Tags.AddUnique(FName("Color.Gray"));
}

void AGrayCube::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    // ✅ 블루프린트에서 덮어쓴 경우에도 실행 시 강제 OFF
    if (MeshComp)
    {
        MeshComp->SetCanEverAffectNavigation(false);
    }
}
