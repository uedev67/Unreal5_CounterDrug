#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

UCLASS()
class COLORATTACK_API UMyGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    // 스테이지 이름과 클리어 여부를 저장하는 맵
    UPROPERTY(BlueprintReadWrite)
    TMap<FName, bool> StageClearMap;

    // 스테이지 클리어 처리
    UFUNCTION(BlueprintCallable)
    void MarkStageCleared(FName StageName);

    // 해당 스테이지가 클리어되었는지 확인
    UFUNCTION(BlueprintCallable)
    bool IsStageCleared(FName StageName) const;
};
