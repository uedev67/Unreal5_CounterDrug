// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ColorAttackGameMode.generated.h"

UCLASS(minimalapi)
class AColorAttackGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AColorAttackGameMode();

	UFUNCTION(BlueprintCallable, Category="Timer")
    void AddTime(int32 ExtraSeconds);

	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowAlertOnTimer(const FText& Message);

	UFUNCTION(BlueprintCallable, Category="UI")
    void PlayHitEffect(); 

    UFUNCTION(BlueprintCallable, Category="Timer")
    int32 GetRemainingTime() const { return RemainingTime; }

	UFUNCTION(BlueprintCallable, Category="Game")
    void HandleGameOver();

    UFUNCTION(BlueprintCallable, Category="Game")
    void GotoLoby();

protected:
	virtual void BeginPlay() override;

	void UpdateTimer();

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> TimerWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category="UI", meta=(AllowPrivateAccess="true"))
	UUserWidget* TimerWidgetInstance;

	FTimerHandle CountdownTimerHandle;

	UPROPERTY()
    UUserWidget* GameOverWidgetInstance = nullptr;

    FTimerHandle RestartTimerHandle;

    UPROPERTY(BlueprintReadOnly, Category="Game")
    bool bIsGameOver = false;

	int32 RemainingTime = 120; 

};



