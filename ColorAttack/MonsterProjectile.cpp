#include "MonsterProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "ColorAttackGameMode.h"
#include "GameFramework/Character.h"


AMonsterProjectile::AMonsterProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    RootComponent = ProjectileMesh;

    ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Overlap);

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->InitialSpeed = 3000.f;
    ProjectileMovementComponent->MaxSpeed = 3000.f;
    ProjectileMovementComponent->ProjectileGravityScale = 0.f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;

    TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailParticles"));
    TrailParticles->SetupAttachment(RootComponent);

    InitialLifeSpan = LifeTime;
}


void AMonsterProjectile::BeginPlay()
{
    Super::BeginPlay();
    ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AMonsterProjectile::OnOverlap);

    if (AActor* MyOwner = GetOwner())
    {
        ProjectileMesh->IgnoreActorWhenMoving(MyOwner, true);
    }
}



void AMonsterProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 플레이어 가져오기
    ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    // 플레이어 맞았을 때만 처리
    if (OtherActor && OtherActor == Player)
    {
        if (AColorAttackGameMode* GM = Cast<AColorAttackGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
        {
            GM->AddTime(-8);  // 타이머 -8초
            GM->PlayHitEffect();   // ⬅ 피격 애니메이션 재생
        }

        Destroy(); // 맞으면 발사체 제거
    }
}
