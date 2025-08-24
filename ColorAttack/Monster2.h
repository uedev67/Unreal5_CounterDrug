#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h" 
#include "Components/BoxComponent.h"
#include "Monster2.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class COLORATTACK_API AMonster2 : public ACharacter
{
    GENERATED_BODY()

public:
    AMonster2();

    UPROPERTY(EditDefaultsOnly, Category="Attack")
    TSubclassOf<AActor> ProjectileClass;  // 여기다가 BP_Projectile 붙이기

protected:
    virtual void BeginPlay() override;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
    UAIPerceptionComponent* PerceptionComp;

public:
    virtual void Tick(float DeltaTime) override;

private:
    
    // 스턴 관련 기능들
    UPROPERTY(VisibleAnywhere, Category="Monster|Collision")
    UBoxComponent* StunZone;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    UAnimMontage* StunMontage;

    bool bIsStunned = false;
    FTimerHandle StunTimerHandle;

    void EndStun();

    UFUNCTION()
    void OnHeadOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
};
