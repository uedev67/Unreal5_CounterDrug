#include "Monster1.h"
#include "EngineUtils.h"  
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Grabber.h"   
#include "PhysicsEngine/PhysicsHandleComponent.h"



AMonster1::AMonster1()
{
    PrimaryActorTick.bCanEverTick = true;
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    MonsterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MonsterMesh"));
    MonsterMesh->SetupAttachment(RootComponent);

    GrabberComponent = CreateDefaultSubobject<UGrabber>(TEXT("GrabberComponent"));
    GrabberComponent->SetupAttachment(RootComponent);
    CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

    CarriedActor = nullptr;

    LowerBodyTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("LowerBodyTrigger"));
    LowerBodyTrigger->SetupAttachment(RootComponent);
    LowerBodyTrigger->SetBoxExtent(FVector(50.f, 50.f, 30.f)); // 하체만 감지
    LowerBodyTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    LowerBodyTrigger->SetCollisionObjectType(ECC_WorldDynamic);
    LowerBodyTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    LowerBodyTrigger->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
}

void AMonster1::BeginPlay()
{
    Super::BeginPlay();
    for (TActorIterator<AActor> It(GetWorld(), AActor::StaticClass()); It; ++It)
    {
        if (It->GetName().Contains("GrayCube"))
        {
            UE_LOG(LogTemp, Warning, TEXT("GrayCube in World: %s"), *It->GetName());
        }
    }

    if (LowerBodyTrigger)
    {
        LowerBodyTrigger->OnComponentBeginOverlap.AddDynamic(this, &AMonster1::OnLowerBodyOverlap);  // 다리 맞으면 스턴 걸리기
    }

    if (GrabberComponent)
    {
        GrabberComponent->Release();   // 혹시 이전 GrabbedObject가 남아 있으면 초기화
    }

}

void AMonster1::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMonster1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

//  몬스터가 회색 큐브 잡기
void AMonster1::GrabObject(AActor* TargetActor)
{
    if (!TargetActor || !GrabberComponent) return;

    // 이미 뭔가 들고 있다면 놓기
    if (CarriedActor)
    {
        DropObject();
    }

    GrabberComponent->GrabSpecificActor(TargetActor);
    CarriedActor = TargetActor;

}

//  몬스터가 큐브 놓기
void AMonster1::DropObject()
{
    if (GrabberComponent)
    {
        GrabberComponent->Release();
    }

    if (CarriedActor)
    {

        // ✅ Drop 후 강제로 Physics 깨워서 Overlap 이벤트가 정상 발생하도록 처리
        if (UPrimitiveComponent* Comp = Cast<UPrimitiveComponent>(CarriedActor->GetRootComponent()))
        {
            Comp->WakeAllRigidBodies();  // Physics Sleep 방지
        }

        CarriedActor = nullptr;
    }
}


void AMonster1::OnLowerBodyOverlap(
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
        UE_LOG(LogTemp, Warning, TEXT("Monster1 is Stunned!!"));

        GetCharacterMovement()->DisableMovement(); // 몬스터1 정지

        GetWorldTimerManager().SetTimer(StunTimerHandle, this, &AMonster1::EndStun, 3.0f, false);
    }
}


void AMonster1::EndStun()
{
    bIsStunned = false;
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);    // 다시 이동 가능
}