#include "Furnace.h"
#include "ColorAttackGameMode.h"
#include "Components/BoxComponent.h"
#include "Red_Character.h" 
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h" 



AFurnace::AFurnace()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    FurnaceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FurnaceMesh"));
    FurnaceMesh->SetupAttachment(RootComponent);

    // 트리거박스 : 큐브 인식 및 점수 판정
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Overlap);

    // 시야박스 : 가까이 와야만 ui가 보이도록 함
    VisibilityBox = CreateDefaultSubobject<UBoxComponent>(TEXT("VisibilityBox"));
    VisibilityBox->SetupAttachment(RootComponent);
    VisibilityBox->SetBoxExtent(FVector(200.f, 200.f, 100.f));          // 원하는 범위
    VisibilityBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    VisibilityBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    VisibilityBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    FurnaceWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FurnaceWidget"));
    FurnaceWidget->SetupAttachment(RootComponent);
    FurnaceWidget->SetWidgetSpace(EWidgetSpace::World); // 화면 고정
    FurnaceWidget->SetVisibility(false); 
}


void AFurnace::BeginPlay()
{
    Super::BeginPlay();


    //  오버랩 이벤트 바인딩
    if (TriggerBox)
    {
        TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFurnace::OnOverlapBegin);
    }

    // 최초 플레이어 캐릭터 위치 조사
    ARed_Character* Player = Cast<ARed_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (Player && VisibilityBox)
    {
        const bool bIsInside = VisibilityBox->IsOverlappingActor(Player);
        FurnaceWidget->SetVisibility(bIsInside);
    }
    else
    {
        FurnaceWidget->SetVisibility(false);
    }

    if (VisibilityBox)
    {
        VisibilityBox->OnComponentBeginOverlap.AddDynamic(this, &AFurnace::OnPlayerEnter);
        VisibilityBox->OnComponentEndOverlap  .AddDynamic(this, &AFurnace::OnPlayerExit);
    }

    OnFurnaceOwnerChanged.AddDynamic(this, &AFurnace::OnOwnerChanged); // 점유 상태에 따른 색 변경

    // 초기 UI 업데이트
    OnFurnaceCountUpdated.Broadcast(RedCount, GrayCount);

    Tags.Add("Full"); // 초기 설정은 만땅이므로 Full 태그를 붙여서 몬스터가 안 오도록 설정.
}


void AFurnace::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


void AFurnace::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!OtherActor) return;

    // 이미 카운트된 액터면 무시
    if (CountedActors.Contains(OtherActor)) return;

    bool bIsValidCube = false;

    //  태그 판별 -> 점수 반영
    if (OtherActor->Tags.Contains(FName("Color.Red")))
    {
        if (RedCount < RequiredCount)
        {
            RedCount++;
            PlayRedCountSFX();
            if (GrayCount > 0) GrayCount--;
            CountedActors.Add(OtherActor);
            Tags.Remove("Full"); // 회색 만땅 해제
            bIsValidCube = true;
        }
    }
    else if (OtherActor->Tags.Contains(FName("Color.Gray")))
    {
        if (GrayCount < RequiredCount)
        {
            GrayCount++;
            if (RedCount > 0) RedCount--;
            CountedActors.Add(OtherActor);
            if (GrayCount >= RequiredCount)
            {
                Tags.Add("Full"); // 회색 만땅 → 몬스터가 피하도록 설정
            }
            bIsValidCube = true;
        }
        else
        {
            Tags.Add("Full");
        }
    }
    else
    {
        return; // 태그 없는 오브젝트는 무시
    }

    //  UI 갱신 이벤트 호출
    OnFurnaceCountUpdated.Broadcast(RedCount, GrayCount);

    //  점령 상태 갱신
    UpdateOwnerState();

    //  용광로에 닿으면 큐브 삭제
    if (bIsValidCube)
    {
        OtherActor->Destroy();
    }
}



void AFurnace::UpdateOwnerState()
{

    //  레드 점령
    if (RedCount >= RequiredCount && CurrentOwner != EFurnaceOwner::Red)
    {
        CurrentOwner = EFurnaceOwner::Red;
        OnFurnaceOwnerChanged.Broadcast(CurrentOwner);
        UE_LOG(LogTemp, Warning, TEXT("Furnace Captured by RED!"));

        // 추가시간 60초 증정
        if (AColorAttackGameMode* GM = GetWorld()->GetAuthGameMode<AColorAttackGameMode>())
        {
        GM->AddTime(60);
        }
        return;
    }
    //  그레이 점령
    else if (GrayCount >= RequiredCount && CurrentOwner != EFurnaceOwner::Gray)
    {
        CurrentOwner = EFurnaceOwner::Gray;
        OnFurnaceOwnerChanged.Broadcast(CurrentOwner);
        UE_LOG(LogTemp, Warning, TEXT("Furnace Captured by GRAY!"));
        return;
    }
 
}   



void AFurnace::OnOwnerChanged(EFurnaceOwner NewOwner)
{
    if (!FurnaceMesh) return;

    switch (NewOwner)
    {
        case EFurnaceOwner::Red:
            FurnaceMesh->SetMaterial(0, RedMaterial);
            PlayRedCaptureSFX();
            break;
        case EFurnaceOwner::Gray:
            FurnaceMesh->SetMaterial(0, GrayMaterial);
            break;
        default:
            break;
    }
}


// 플레이어 접근
void AFurnace::OnPlayerEnter(
    UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA<ARed_Character>())
    {
        FurnaceWidget->SetVisibility(true);
    }
}


void AFurnace::OnPlayerExit(
    UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor->IsA<ARed_Character>())
    {
        FurnaceWidget->SetVisibility(false);
    }
}


void AFurnace::PlayRedCaptureSFX()
{
    if (RedCaptureSFX)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            RedCaptureSFX,
            GetActorLocation(),
            /*VolumeMultiplier=*/1.0f,
            /*PitchMultiplier=*/1.0f,
            /*StartTime=*/0.0f,
            /*AttenuationSettings=*/CaptureAttenuation
        );
    }
}


void AFurnace::PlayRedCountSFX()
{
    if (RedCountSFX)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            RedCountSFX,
            GetActorLocation(),
            /*VolumeMultiplier=*/1.0f,
            /*PitchMultiplier=*/1.0f,
            /*StartTime=*/0.0f,
            /*AttenuationSettings=*/CaptureAttenuation
        );
    }
}

