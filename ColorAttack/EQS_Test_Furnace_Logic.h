#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EQS_Test_Furnace_Logic.generated.h"

UCLASS()
class COLORATTACK_API UEQS_Test_Furnace_Logic : public UEnvQueryTest
{
    GENERATED_BODY()

public:
    UEQS_Test_Furnace_Logic();

protected:
    virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
