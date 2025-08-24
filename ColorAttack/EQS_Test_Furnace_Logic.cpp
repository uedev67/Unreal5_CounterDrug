#include "EQS_Test_Furnace_Logic.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "GameFramework/Actor.h"



UEQS_Test_Furnace_Logic::UEQS_Test_Furnace_Logic()
{
    Cost = EEnvTestCost::Low; 
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
    TestPurpose = EEnvTestPurpose::Filter;   // 필터링 용도
    FilterType = EEnvTestFilterType::Match;  // 매치하면 통과
}



void UEQS_Test_Furnace_Logic::RunTest(FEnvQueryInstance& QueryInstance) const
{
    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        AActor* CandidateActor = GetItemActor(QueryInstance, It.GetIndex());
        if (!CandidateActor)
        {
            It.SetScore(TestPurpose, FilterType, 0.0f, 1.0f); // 최저 점수를 줘서 거르기
            continue;
        }

        if (CandidateActor->Tags.Contains("Full"))
        {
            It.SetScore(TestPurpose, FilterType, 0.0f, 1.0f); // 최저 점수를 줘서 거르기
        }
        else
        {
            // 만땅이 아니라면 통과 (점수는 1.0)
            It.SetScore(TestPurpose, FilterType, 1.0f, 1.0f);
        }
    }
}
