#include "MyGameInstance.h"

void UMyGameInstance::MarkStageCleared(FName StageName)
{
    StageClearMap.FindOrAdd(StageName) = true;
}


bool UMyGameInstance::IsStageCleared(FName StageName) const
{
    if (const bool* bCleared = StageClearMap.Find(StageName))
    {
        return *bCleared;
    }
    return false;
}
