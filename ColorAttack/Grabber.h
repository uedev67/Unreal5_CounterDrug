#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"




UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COLORATTACK_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:
	UGrabber();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Grab();

	UFUNCTION(BlueprintCallable)
	void Release();

	UFUNCTION(BlueprintCallable)
	void SetHoldDistance(float NewDistance);

	UFUNCTION(BlueprintCallable)
	void ResetHoldDistance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grab")
	float HoldDistance = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grab")
	float DefaultHoldDistance = 400.0f;

	// ✅ FName 기반으로 변경
	void SetOwnerColorTag(const FName& NewTag);

	UFUNCTION(BlueprintCallable)
    void GrabSpecificActor(AActor* TargetActor); //몬스터 ai 잡기 함수

private:
	UPROPERTY(EditAnywhere, Category="Grab")
	float MaxDistance = 1800.0f;

	UPROPERTY(EditAnywhere, Category="Grab")
	float GrabRadius = 70.0f;

	UPROPERTY()
	AActor* GrabbedObject;

	// ✅ 단순 문자열 태그 비교
	UPROPERTY()
	FName OwnerColorTag; 

	/** 내부 함수 */
	UPhysicsHandleComponent* GetPhysicsHandle() const;
	bool GetGrabbableInReach(FHitResult& OutHitResult) const;

	/** 태그 비교용 함수 */
	bool CanGrabActor(AActor* TargetActor) const;
};
