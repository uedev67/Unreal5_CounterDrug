#include "Red_Character.h"
#include "Grabber.h"                      
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"



ARed_Character::ARed_Character()
{
    //  FName으로 설정
    CharacterColorTag = FName("Color.Red");

    PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

    GrabberComponent = CreateDefaultSubobject<UGrabber>(TEXT("Grabber"));
    GrabberComponent->SetupAttachment(RootComponent); 

    if (GrabberComponent)
    {
        GrabberComponent->SetOwnerColorTag(CharacterColorTag);
    }
}

void ARed_Character::BeginPlay()
{
    Super::BeginPlay();

    if (GrabberComponent)
    {
        GrabberComponent->SetOwnerColorTag(CharacterColorTag);
    }
}


void ARed_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // 물건 잡기
        if (GrabAction)
        {
            EnhancedInput->BindAction(GrabAction, ETriggerEvent::Started, this, &ARed_Character::Grab);
            EnhancedInput->BindAction(GrabAction, ETriggerEvent::Completed, this, &ARed_Character::Release);
        }

        // 물건 잡은 상태로 Push
        if (PushAction)
        {
            EnhancedInput->BindAction(PushAction, ETriggerEvent::Started, this, &ARed_Character::PushStart);
            EnhancedInput->BindAction(PushAction, ETriggerEvent::Completed, this, &ARed_Character::PushEnd);
        }
    }
     
}

void ARed_Character::Grab()
{
    if (GrabberComponent)
    {
        GrabberComponent->Grab();
    }
}

void ARed_Character::Release()
{
    if (GrabberComponent)
    {
        GrabberComponent->Release();
    }
}

// Q키를 눌러서 Push
void ARed_Character::PushStart()
{
    if (GrabberComponent)
    {
        GrabberComponent->SetHoldDistance(1400.0f); // 앞으로 푸시
    }
}

void ARed_Character::PushEnd()
{
    if (GrabberComponent)
    {
        GrabberComponent->ResetHoldDistance(); // 원래로 복귀
    }
}

