#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Monster1.generated.h"



class UGrabber;  

UCLASS()
class COLORATTACK_API AMonster1 : public ACharacter
{
    GENERATED_BODY()

public:
    AMonster1();
  
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void GrabObject(AActor* TargetActor);
    void DropObject();

    //  현재 들고 있는 액터 확인용
    AActor* GetCarriedActor() const { return CarriedActor; }

protected:
    virtual void BeginPlay() override;

private:
    // 몬스터의 외형 메시
    UPROPERTY(VisibleAnywhere, Category="Monster")
    UStaticMeshComponent* MonsterMesh;

    //  Grabber 컴포넌트 (PhysicsHandle 포함)
    UPROPERTY(VisibleAnywhere, Category="Monster")
    UGrabber* GrabberComponent;

    UPROPERTY()
    AActor* CarriedActor;

    // 스턴 관련 기능들(레드 큐브를 하체에 맞을 시)
    UPROPERTY(VisibleAnywhere, Category="Monster|Collision")
    UBoxComponent* LowerBodyTrigger;

    bool bIsStunned = false;
    FTimerHandle StunTimerHandle;

    void EndStun();

    UFUNCTION()
    void OnLowerBodyOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
};


