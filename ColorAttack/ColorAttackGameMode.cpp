// Copyright Epic Games, Inc. All Rights Reserved.

#include "ColorAttackGameMode.h"
#include "ColorAttackCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"


AColorAttackGameMode::AColorAttackGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

}


void AColorAttackGameMode::BeginPlay()
{
    Super::BeginPlay();

    //  타이머 UI 화면에 띄움
    if (TimerWidgetClass)
    {
        TimerWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), TimerWidgetClass);
        if (TimerWidgetInstance)
        {
            TimerWidgetInstance->AddToViewport();
        }
    }

    //  1초마다 타이머 감소
    RemainingTime = 120; 
    GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &AColorAttackGameMode::UpdateTimer, 1.0f, true);
}


// 중앙 상단 타이머 WBP제어
void AColorAttackGameMode::UpdateTimer()
{
    RemainingTime = FMath::Max(0, RemainingTime - 1);

    if (TimerWidgetInstance)
    {
        if (UFunction* UpdateFunc = TimerWidgetInstance->FindFunction(FName("UpdateTimerText")))
        {
            struct FTimerParam { int32 Time; } Param{ RemainingTime };
            TimerWidgetInstance->ProcessEvent(UpdateFunc, &Param);
        }
    }

    if (RemainingTime == 0 && !bIsGameOver)
    {
        HandleGameOver();
    }
}


// 타이머 시간 관리 함수 : 시간 추가하거나 빼거나 둘 다 가능.
void AColorAttackGameMode::AddTime(int32 ExtraSeconds)
{
    RemainingTime = FMath::Max(0, RemainingTime + ExtraSeconds);

    //  UI 즉시 갱신
    if (TimerWidgetInstance)
    {
        UFunction* UpdateFunc = TimerWidgetInstance->FindFunction(FName("UpdateTimerText"));
        if (UpdateFunc)
        {
            struct FTimerParam { int32 Time; };
            FTimerParam Param;
            Param.Time = RemainingTime;
            TimerWidgetInstance->ProcessEvent(UpdateFunc, &Param);
        }

        UFunction* PlayAnimFunc = TimerWidgetInstance->FindFunction(FName("PlayAddTimeAnim"));
        if (PlayAnimFunc)
        {
            TimerWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
        }
    }

    if (RemainingTime == 0 && !bIsGameOver)
    {
        HandleGameOver();
    }
}


// 피격 시 일시적으로 화면이 붉어지는 애니메이션 
void AColorAttackGameMode::PlayHitEffect()
{
    if (!TimerWidgetInstance) return;

    if (UFunction* PlayHitFunc = TimerWidgetInstance->FindFunction(FName("PlayGetHitAnim")))
    {
        TimerWidgetInstance->ProcessEvent(PlayHitFunc, nullptr);
    }
}


// cctv 발각 경고 메시지
void AColorAttackGameMode::ShowAlertOnTimer(const FText& Message)
{
    if (!TimerWidgetInstance) return;

    if (UFunction* Func = TimerWidgetInstance->FindFunction(FName("Show Alert CCTV")))
    {
        struct { FText Text; } Params{ Message };
        TimerWidgetInstance->ProcessEvent(Func, &Params);
    }
}


// 게임오버 관리함수
void AColorAttackGameMode::HandleGameOver()
{
    if (bIsGameOver) return;
    bIsGameOver = true;

    // 타이머 정지
    GetWorldTimerManager().ClearTimer(CountdownTimerHandle);


    // 입력을 UI로 전환 + 일시정지
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    {
        PC->SetShowMouseCursor(true);
        PC->SetInputMode(FInputModeUIOnly());
    }

    // 3초 후 로비로 이동
    GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &AColorAttackGameMode::GotoLoby, 3.0f, false);
}



void AColorAttackGameMode::GotoLoby()
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    {
        PC->SetPause(false);
        PC->SetShowMouseCursor(false);
        PC->SetInputMode(FInputModeGameOnly());
    }

    UGameplayStatics::OpenLevel(this, FName(TEXT("LV_Roby")));  
}
