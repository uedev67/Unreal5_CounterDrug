#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelPortal.generated.h"

class UBoxComponent;
class UUserWidget;

UCLASS()
class COLORATTACK_API ALevelPortal : public AActor
{
    GENERATED_BODY()

public:
    ALevelPortal();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);                    

public:
    UPROPERTY(VisibleAnywhere)
    UBoxComponent* TriggerBox;

    UPROPERTY(EditDefaultsOnly, Category="UI")
    TSubclassOf<UUserWidget> LevelSelectUIClass;

private:
    UPROPERTY()
    UUserWidget* ActiveWidget;
};
