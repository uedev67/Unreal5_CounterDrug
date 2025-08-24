#include "LevelPortal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

ALevelPortal::ALevelPortal()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;

    TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
    TriggerBox->SetGenerateOverlapEvents(true);
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelPortal::OnOverlapBegin);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ALevelPortal::OnOverlapEnd);
}

void ALevelPortal::BeginPlay()
{
    Super::BeginPlay();
}

void ALevelPortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                   bool bFromSweep, const FHitResult& SweepResult)
{
    if (ActiveWidget) return;

    if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
    {
        if (LevelSelectUIClass)
        {
            APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PC)
            {
                ActiveWidget = CreateWidget<UUserWidget>(PC, LevelSelectUIClass);
                if (ActiveWidget)
                {
                    ActiveWidget->AddToViewport();

                    PC->bShowMouseCursor = true;

                    FInputModeGameAndUI InputMode;
                    InputMode.SetWidgetToFocus(ActiveWidget->TakeWidget());
                    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                    PC->SetInputMode(InputMode);
                }
            }
        }
    }	
}


void ALevelPortal::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
    {
        if (ActiveWidget)
        {
            ActiveWidget->RemoveFromParent();
            ActiveWidget = nullptr;

            APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PC)
            {
                PC->bShowMouseCursor = false;

                // UI 제거 후 게임 전용 입력으로 복귀
                FInputModeGameOnly InputMode;
                PC->SetInputMode(InputMode);
            }
        }
    }
}
