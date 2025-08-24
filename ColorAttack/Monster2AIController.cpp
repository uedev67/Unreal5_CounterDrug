// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster2AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


void AMonster2AIController::BeginPlay()
{
    Super::BeginPlay();

    if (BehaviorTreeAsset)
    {
        UE_LOG(LogTemp, Warning, TEXT("Monster2AIController: Running BT"));
        UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComp);
        RunBehaviorTree(BehaviorTreeAsset);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Monster2AIController: BehaviorTreeAsset is NULL!"));
    }
}