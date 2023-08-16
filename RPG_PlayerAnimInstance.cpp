
#include "RPG_PlayerAnimInstance.h"

#include "RPG_PlayerCharacter.h"
#include "RPG_PlayerController.h"
#include "Kismet/GameplayStatics.h"

void URPG_PlayerAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();
	PlayerCharacter = Cast<ARPG_PlayerCharacter>(TryGetPawnOwner());
	PlayerCharacterController = Cast<ARPG_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void URPG_PlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds) {
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if (!PlayerCharacter) {
		return;
	}
	if (PlayerCharacterController) {
		VerticalAxisValue = PlayerCharacterController->GetInputAxisValue("Vertical");
		HorizontalAxisValue = PlayerCharacterController->GetInputAxisValue("Horizontal");
	}
	Speed = PlayerCharacter->GetPlayerSpeed();
	bIsInAir = PlayerCharacter->GetPlayerIsInAir();
	bIsMoving = PlayerCharacter->GetPlayerIsMoving();
	bIsFalling = PlayerCharacter->GetPlayerIsFalling();
	bIsCrouching = PlayerCharacter->GetPlayerIsCrouching();
	FallingSpeed = PlayerCharacter->GetPlayerFallingSpeed();
}
