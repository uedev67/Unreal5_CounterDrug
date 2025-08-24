#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrayCube.generated.h"

UCLASS()
class COLORATTACK_API AGrayCube : public AActor
{
    GENERATED_BODY()

public:
    AGrayCube();

protected:
    virtual void PostInitializeComponents() override;

    /** 네비게이션 영향 안 주는 메쉬 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh", meta=(AllowPrivateAccess="true"))
    UStaticMeshComponent* MeshComp;
};
