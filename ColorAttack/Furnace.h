#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundAttenuation.h"
#include "Furnace.generated.h"



//  점령 상태 Enum
UENUM(BlueprintType)
enum class EFurnaceOwner : uint8
{
    Neutral UMETA(DisplayName="Neutral"),
    Red     UMETA(DisplayName="Red"),
    Gray    UMETA(DisplayName="Gray")
};

//  UI 갱신용 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFurnaceCountUpdated, int32, RedCount, int32, GrayCount);

//  색상 변경용 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFurnaceOwnerChanged, EFurnaceOwner, NewOwner);

UCLASS()
class COLORATTACK_API AFurnace : public AActor
{
    GENERATED_BODY()
	
public:	
    AFurnace();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    UFUNCTION()
    void OnPlayerEnter(
        UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult
    );

    UFUNCTION()
    void OnPlayerExit(
        UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
    );

    //  점유 팀 확인 함수
    void UpdateOwnerState();

public:	
    virtual void Tick(float DeltaTime) override;

    //  위젯/BP에서 RequiredCount, 현재 Red/Gray 카운트 확인 가능
    UFUNCTION(BlueprintCallable, Category="Furnace")
    int32 GetRedCount() const { return RedCount; }

    UFUNCTION(BlueprintCallable, Category="Furnace")
    int32 GetGrayCount() const { return GrayCount; }

    UFUNCTION(BlueprintCallable, Category="Furnace")
    EFurnaceOwner GetCurrentOwner() const { return CurrentOwner; }

    UFUNCTION()  
    void OnOwnerChanged(EFurnaceOwner NewOwner);

    UFUNCTION(BlueprintCallable, Category="Furnace|Audio")
    void PlayRedCaptureSFX();  // 레드팀 점유 시, 효과음 재생

    UFUNCTION(BlueprintCallable, Category="Furnace|Audio")
    void PlayRedCountSFX();  // 레드팀 용광로에 드럼 갖다놓을 때마다, 효과음 재생

protected:
    //  트리거 영역
    UPROPERTY(VisibleAnywhere, Category="Furnace|Components")
    UBoxComponent* TriggerBox;

    // 플레이어가 접근해야만 점수 ui가 보이도록 설정
    UPROPERTY(VisibleAnywhere, Category="Furnace|Components")
    UBoxComponent* VisibilityBox;

    //  외형 메쉬
    UPROPERTY(VisibleAnywhere, Category="Furnace|Components")
    UStaticMeshComponent* FurnaceMesh;

    UPROPERTY(EditAnywhere, Category="Furnace|Appearance")
    UMaterialInterface* RedMaterial;

    UPROPERTY(EditAnywhere, Category="Furnace|Appearance")
    UMaterialInterface* GrayMaterial;

    //  UI 위젯 (6:0 표시)
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Furnace|Components", meta=(AllowPrivateAccess="true"))
	UWidgetComponent* FurnaceWidget;

    //  용광로 당 요구하는 큐브 수(원하는대로 조정 가능)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Furnace|Settings")
    int32 RequiredCount = 4;

    //  현재 점령 상태
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Furnace|State")
    EFurnaceOwner CurrentOwner = EFurnaceOwner::Neutral;

    // SFX : 플레이어가 용광로 점유 시에 나는 효과음
    UPROPERTY(EditAnywhere, Category="Furnace|Audio")
    USoundBase* RedCaptureSFX = nullptr;

    // SFX : 플레이어가 용광로에 드럼통 갖다놓을 때마다 나는 효과음
    UPROPERTY(EditAnywhere, Category="Furnace|Audio")
    USoundBase* RedCountSFX = nullptr;


    UPROPERTY(EditAnywhere, Category="Furnace|Audio")
    USoundAttenuation* CaptureAttenuation = nullptr;

    //  초기 세팅값 : red 0 : 4 gray
    int32 RedCount = 0;
    int32 GrayCount = RequiredCount;

    //  중복 카운트 방지
    TSet<TWeakObjectPtr<AActor>> CountedActors;

public:
    //  UI 갱신 이벤트 : 위젯 블프랑 연동
    UPROPERTY(BlueprintAssignable, Category="Furnace|Events")
    FOnFurnaceCountUpdated OnFurnaceCountUpdated;

    //  색상 변경 이벤트
    UPROPERTY(BlueprintAssignable, Category="Furnace|Events")
    FOnFurnaceOwnerChanged OnFurnaceOwnerChanged;
};
