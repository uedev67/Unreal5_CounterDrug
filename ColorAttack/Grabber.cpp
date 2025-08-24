#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// Owner의 첫 번째 태그를 FName으로 저장
    if (AActor* OwnerActor = GetOwner())
    {
        if (OwnerActor->Tags.Num() > 0)
        {
            OwnerColorTag = OwnerActor->Tags[0];
            UE_LOG(LogTemp, Warning, TEXT("Grabber OwnerColorTag = %s"), *OwnerColorTag.ToString());
        }
    }
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
    if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
    {
        AActor* OwnerActor = GetOwner();
        if (!OwnerActor) return;

        FVector TargetLocation;
        FRotator TargetRotation;

        if (APlayerController* PC = Cast<APlayerController>(OwnerActor->GetInstigatorController()))
        {
            // ✅ 플레이어면 카메라 기준
            FVector CameraLocation;
            FRotator CameraRotation;
            PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

            TargetLocation = CameraLocation + CameraRotation.Vector() * HoldDistance;
            TargetRotation = CameraRotation;
        }
        else
        {
            // ✅ AI/몬스터면 Actor Forward 기준
            TargetLocation = OwnerActor->GetActorLocation() + OwnerActor->GetActorForwardVector() * HoldDistance;
            TargetRotation = OwnerActor->GetActorRotation();
        }

        PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, TargetRotation);
    }
}



void UGrabber::Grab()
{
    UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
    if (!PhysicsHandle)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ PhysicsHandle is NULL!"));
        return;
    }

    FHitResult HitResult;
    if (GetGrabbableInReach(HitResult))
    {
        AActor* HitActor = HitResult.GetActor();
        UPrimitiveComponent* HitComponent = HitResult.GetComponent();

        if (!HitActor || HitActor == GetOwner()) return;

        //  태그 체크
        if (!CanGrabActor(HitActor))
        {
            return;
        }

        bool bSimulating = HitComponent->IsSimulatingPhysics();

        if (!bSimulating)
        {
            UE_LOG(LogTemp, Error, TEXT("❌ %s is NOT simulating physics! Can't grab."),
                *HitActor->GetName());
            return;
        }


        HitComponent->WakeAllRigidBodies();
        PhysicsHandle->GrabComponentAtLocationWithRotation(
            HitComponent,
            NAME_None,
            HitComponent->GetComponentLocation(), // Mesh 중심 좌표 사용
            HitComponent->GetComponentRotation()
        );


        GrabbedObject = HitActor;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No grabbable found!"));
    }
}


void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->ReleaseComponent();
		GrabbedObject = nullptr;
	}
}


// Q키를 눌러 Push하기
void UGrabber::SetHoldDistance(float NewDistance)
{
    HoldDistance = NewDistance;
}

void UGrabber::ResetHoldDistance()
{
    HoldDistance = DefaultHoldDistance;
}




UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!Result)
	{
		UE_LOG(LogTemp, Warning, TEXT("Grabber needs a UPhysicsHandleComponent!"));
	}
	return Result;
}


bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return false;

    
    FVector CameraLocation;
    FRotator CameraRotation;
    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

    
    FVector Start = CameraLocation;
    FVector End = Start + (CameraRotation.Vector() * MaxDistance);

    //DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f);
    //DrawDebugSphere(GetWorld(), End, GrabRadius, 10, FColor::Blue, false, 1.0f);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start, End,
		FQuat::Identity,
		ECC_Visibility,
		Sphere
	);
}


//  FName 기반 태그 비교
bool UGrabber::CanGrabActor(AActor* TargetActor) const
{
    if (!TargetActor) return false;

    for (const FName& Tag : TargetActor->Tags)
    {
        if (Tag == OwnerColorTag)
            return true;
    }
    return false;
}

void UGrabber::SetOwnerColorTag(const FName& NewTag)
{
    OwnerColorTag = NewTag;
}


// 몬스터 ai 전용 함수
void UGrabber::GrabSpecificActor(AActor* TargetActor)
{
    if (!TargetActor) return;

    UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
    if (!PhysicsHandle) return;

    // ✅ Root가 SceneComponent일 수도 있으므로 Mesh 직접 찾기
    UPrimitiveComponent* TargetComp = Cast<UPrimitiveComponent>(TargetActor->GetRootComponent());
    if (!TargetComp || !TargetComp->IsSimulatingPhysics())
    {
        TargetComp = TargetActor->FindComponentByClass<UStaticMeshComponent>();
        if (!TargetComp)
        {
            return;
        }
    }

    // ✅ 강제로 SimulatePhysics 켜기
    if (!TargetComp->IsSimulatingPhysics())
    {
        TargetComp->SetSimulatePhysics(true);
        TargetComp->WakeAllRigidBodies();
    }

    FVector GrabLoc = TargetComp->GetComponentLocation();
    FRotator GrabRot = TargetComp->GetComponentRotation();

    PhysicsHandle->GrabComponentAtLocationWithRotation(
        TargetComp,
        NAME_None,
        GrabLoc,
        GrabRot
    );
        
    GrabbedObject = TargetActor;
}


