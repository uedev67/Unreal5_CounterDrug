#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterProjectile.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class COLORATTACK_API AMonsterProjectile : public AActor
{
    GENERATED_BODY()

public:
    AMonsterProjectile();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditDefaultsOnly, Category="Projectile")
    UStaticMeshComponent* ProjectileMesh;

    UPROPERTY(VisibleAnywhere, Category="Projectile")
    UProjectileMovementComponent* ProjectileMovementComponent;

    UPROPERTY(VisibleAnywhere, Category="Projectile")
    UParticleSystemComponent* TrailParticles;

    UPROPERTY(EditAnywhere, Category="Projectile")
    float LifeTime = 5.0f; // 명중을 못 해도 5초 뒤에 사라지게 설계

    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
           
};
