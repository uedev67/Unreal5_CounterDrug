// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Monster2AIController.generated.h"

/**
 * 
 */
UCLASS()
class COLORATTACK_API AMonster2AIController : public AAIController
{
	GENERATED_BODY()

public:

    virtual void BeginPlay() override;

protected:
    // Behavior Tree 실행만 남김
    UPROPERTY(EditDefaultsOnly, Category="AI")
    UBehaviorTree* BehaviorTreeAsset;	

    UPROPERTY()
    UBlackboardComponent* BlackboardComp;
	
};
