#pragma once

#include "CoreMinimal.h"
#include "ColorAttackCharacter.h"
#include "GameplayTagContainer.h"
#include "Red_Character.generated.h"



class UGrabber;
class UPhysicsHandleComponent;
class AClue;

UCLASS()
class COLORATTACK_API ARed_Character : public AColorAttackCharacter
{
	GENERATED_BODY()
	
public:
	ARed_Character();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ability|Grab")
	UGrabber* GrabberComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ability|Grab")
	UPhysicsHandleComponent* PhysicsHandleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Color")
	FName CharacterColorTag;

	// 게임 시작 시 호출
	virtual void BeginPlay() override;

	// 입력 바인딩
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Grab();
	void Release();
	void PushStart();
    void PushEnd();

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
    UInputAction* GrabAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	UInputAction* PushAction;
	
};
