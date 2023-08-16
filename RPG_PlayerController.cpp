
#include "RPG_PlayerController.h"

#include "RPG_PlayerAnimInstance.h"
#include "RPG_PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"


// Begin Play
void ARPG_PlayerController::BeginPlay() {
	Super::BeginPlay();
	PlayerCharacter = Cast<ARPG_PlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

// Setup the input system
void ARPG_PlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	// Bind Actions
	InputComponent->BindAction("SpaceBar", IE_Pressed, this, &ARPG_PlayerController::SpaceBarPressedHandler);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &ARPG_PlayerController::LeftShiftPressedHandler);
	InputComponent->BindAction("Crouch", IE_Pressed, this, &ARPG_PlayerController::LeftControlPressedHandler);
	InputComponent->BindAction("Attack", IE_Pressed, this, &ARPG_PlayerController::AttackPressedHandler);
	// Bind Axis
	InputComponent->BindAxis("Vertical", this, &ARPG_PlayerController::VerticalAxisHandler);
	InputComponent->BindAxis("Horizontal", this, &ARPG_PlayerController::HorizontalAxisHandler);
	InputComponent->BindAxis("Mouse X", this, &ARPG_PlayerController::MouseXHandler);
	InputComponent->BindAxis("Mouse Y", this, &ARPG_PlayerController::MouseYHandler);
}

void ARPG_PlayerController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	if (PlayerCharacter == nullptr) {
		return;
	}
	PlayerCharacter->UpdatePlayerProperties();
}


// Input Handler Functions:
void ARPG_PlayerController::SpaceBarPressedHandler() {
	if (PlayerCharacter == nullptr) {
		return;
	}
	PlayerCharacter->HandleClimb();
}

void ARPG_PlayerController::LeftShiftPressedHandler() {
	if (PlayerCharacter == nullptr) {
		return;
	}
	PlayerCharacter->ToggleSprinting();
}

void ARPG_PlayerController::LeftControlPressedHandler() {
	if (PlayerCharacter == nullptr) {
		return;
	}
	PlayerCharacter->ToggleCrouching();
}

void ARPG_PlayerController::AttackPressedHandler() {
	if (PlayerCharacter == nullptr) {
		return;
	}
	PlayerCharacter->Attack();
}

void ARPG_PlayerController::VerticalAxisHandler(float Value) {
	MoveForward(Value);
}

void ARPG_PlayerController::HorizontalAxisHandler(float Value) {
	MoveSideways(Value);
}

void ARPG_PlayerController::MouseXHandler(float Value) {
	if (PlayerCharacter == nullptr) {
		return;
	}
	PlayerCharacter->AddControllerYawInput(Value);
}

void ARPG_PlayerController::MouseYHandler(float Value) {
	if (PlayerCharacter == nullptr) {
		return;
	}
	PlayerCharacter->AddControllerPitchInput(Value);
}


// Player Movement Functions:
/**
 * @brief Allow the player to move forwards and back
 * @param VerticalAxisValue 
 */
void ARPG_PlayerController::MoveForward(float VerticalAxisValue) {
	if (PlayerCharacter == nullptr) {
		return;
	}
	// Get only the rotation (orientation) of the Controller about the Z-Axis (up axis)
	const FRotator YawRotation {0, GetControlRotation().Yaw, 0};
	// Get the forward direction of the controller (x)
	const FVector ForwardVector {FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X)};
	PlayerCharacter->AddMovementInput(ForwardVector, VerticalAxisValue);
}

/**
 * @brief Allow the player to move left and right
 * @param HorizontalAxisValue 
 */
void ARPG_PlayerController::MoveSideways(float HorizontalAxisValue) {
	if (PlayerCharacter == nullptr) {
		return;
	}
	// Get only the rotation (orientation) of the Controller about the Z-Axis (up axis)
	const FRotator YawRotation {0, GetControlRotation().Yaw, 0};
	// Get the right direction of the controller (y)
	const FVector RightVector {FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y)};
	PlayerCharacter->AddMovementInput(RightVector, HorizontalAxisValue);
}


