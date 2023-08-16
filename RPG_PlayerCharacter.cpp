
#include "RPG_PlayerCharacter.h"

#include "Assassination.h"
#include "MotionWarpingComponent.h"
#include "RPG_PlayerAnimInstance.h"
#include "RPG_PlayerHUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GenericPlatform/GenericPlatformCrashContext.h"

// Constructor 
ARPG_PlayerCharacter::ARPG_PlayerCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	DesiredMaxWalkSpeed = WalkSpeed;
	DesiredTargetArmLength = NormalTargetArmLength;
	
	PlayerSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	PlayerSpringArmComponent->SetupAttachment(RootComponent);
	PlayerSpringArmComponent->TargetArmLength = DesiredTargetArmLength;
	PlayerSpringArmComponent->bUsePawnControlRotation = true;
	PlayerSpringArmComponent->bEnableCameraRotationLag = true;
	PlayerSpringArmComponent->CameraRotationLagSpeed = 10.f;

	PlayerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	PlayerCameraComponent->SetupAttachment(PlayerSpringArmComponent, USpringArmComponent::SocketName);
	PlayerCameraComponent->bUsePawnControlRotation = false;

	PlayerMotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Motion Warping Component"));
	
}

// Called to bind functionality to input (Done in Player Controller Class)
void ARPG_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


// Begin Play
void ARPG_PlayerCharacter::BeginPlay() {
	Super::BeginPlay();
	GetComponentReferences();
	ConfigurePlayerMovementAndRotationParams();
	AddHUDElementsToViewport();
	StartPlayerStatsUpdateTimer();
}

// Tick
void ARPG_PlayerCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	InterpolateMaxWalkSpeed(DeltaTime);
	InterpolateSpringArmLength(DeltaTime);
	UpdatePlayerHUD_Tick(DeltaTime);
	
}




// IHealth Interface Implementations:
void ARPG_PlayerCharacter::IncreaseHealth_Implementation(float Health) {
	IHealth::IncreaseHealth_Implementation(Health);
	CurrentHealth += Health;
	if (CurrentHealth >= MaxHealth) {
		CurrentHealth = MaxHealth;
	}
}

void ARPG_PlayerCharacter::DecreaseHealth_Implementation(float Health) {
	IHealth::DecreaseHealth_Implementation(Health);
	CurrentHealth -= Health;
	if (CurrentHealth <= 0.f) {
		CurrentHealth = 0.f;
	}
}

void ARPG_PlayerCharacter::CharacterDead_Implementation() {
	IHealth::CharacterDead_Implementation();
	// todo..																							ragdoll the player ------------------------------------ //
}


// IAssassination Interface Implementations
FEnemyReferencePosition ARPG_PlayerCharacter::Stealth_BackstabAssassinate_Implementation() {
	return FEnemyReferencePosition {
		FVector::ZeroVector,
		FRotator::ZeroRotator
	};
}


// Public Functions:
/**
 * @brief Updates the state of the Player
 */
void ARPG_PlayerCharacter::UpdatePlayerProperties() {
	if (PlayerCharacterMovementComponent == nullptr) {
		return;
	}
	Speed = GetCharacterLaterSpeed();
	FallingSpeed = GetCharacterFallingSpeed();
	bIsMoving = CheckIfCharacterIsMoving();
	bIsFalling = PlayerCharacterMovementComponent->IsFalling();
}



// Movement Handling Functions
void ARPG_PlayerCharacter::HandleCrouch() {
	bIsSprinting = false;
	SetMaxWalkSpeed(CrouchedSpeed);
	DesiredTargetArmLength = CrouchedTargetArmLength;												
}
void ARPG_PlayerCharacter::HandleWalk() {
	bIsSprinting = false;
	SetMaxWalkSpeed(WalkSpeed);
	DesiredTargetArmLength = NormalTargetArmLength;
}
void ARPG_PlayerCharacter::ToggleSprinting() {
	if (!CanSprint) {
		// Player not allowed to sprint
		return;
	}
	if (PlayerAnimInstance != nullptr && PlayerCharacterMovementComponent != nullptr) {		
		// If player IS crouching, come out of crouching state, and go to walk/sprint
		bIsCrouching = false;
		bIsSprinting = !bIsSprinting;
		SetMaxWalkSpeed(bIsSprinting == true ? SprintSpeed : WalkSpeed);
		DesiredTargetArmLength = NormalTargetArmLength;													
		
	} else {
		UE_LOG(LogTemp, Error, TEXT(" !! NULL reference in ToggleSprinting()"));
	}
}
void ARPG_PlayerCharacter::ToggleCrouching() {
	if (bIsFalling == true) {
		bIsCrouching = false;
		return;
	}
	bIsCrouching = !bIsCrouching;
	if (bIsCrouching == true) {
		HandleCrouch();
	} else {
		HandleWalk();
	}
}


/**
 * @brief Handles the Vault/Mantle/Climb functionality of the Player
 */
void ARPG_PlayerCharacter::HandleClimb() {
	if (bIsClimbing == true) {
		UE_LOG(LogTemp, Display, TEXT(" >> Already in Climb State"));
		return;
	}
	if (bIsFalling == true) {
		UE_LOG(LogTemp, Warning, TEXT(" >> Can't enter Climb State mid air!"));
		return;
	}
	
	FHitResult HitResult{};
	bool HitSomething {false};
	bool FoundClimbableObject {false};
	FVector LastHitLocation {FVector::ZeroVector};
	int ClimbHeight {0};

	
	// Get only the rotation (orientation) of the Controller about the Z-Axis (up axis)
	const FRotator YawRotation {0, Controller->GetControlRotation().Yaw, 0};
	// Get the forward direction of the controller (x)
	const FVector CameraForwardVector {FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X)};
	
	
	// Decide what to do: Nothing / Vault / Mantle / Climb
	// Send forward rays at incremental step heights
	for (int i{0}; i < MaxUpwardTraces; i++) {
		TraceStartVector = GetActorLocation() + FVector(0, 0, ForwardTraceHeightOffset * i);
		TraceEndVector = TraceStartVector + (GetActorForwardVector()/*CameraForwardVector*/ * (HitSomething == true ? MaxSubsequentTraceForwardDistance : MaxTraceForwardDistance));
		// Perform a sweep from start vector to end vector
		HitSomething = World->SweepSingleByChannel(
			HitResult,
			TraceStartVector,
			TraceEndVector,
			FQuat::Identity,
			ECC_Visibility,
			FCollisionShape::MakeSphere(ForwardTraceSphereRadius)
		);
		if (DrawDebugVisuals) {
			DrawDebugLine(World, TraceStartVector, TraceEndVector, FColor::Yellow, false, 3.f);
			DrawDebugSphere(World, HitResult.Location, ForwardTraceSphereRadius, 10, FColor::Yellow, false, 3.f);
		}
		if (HitSomething == true) {
			// Trace hit an object			
			if (HitResult.GetActor()->ActorHasTag("Climb") == true) {				
				// Found a climb marker: break out of loop
				LastHitLocation = HitResult.ImpactPoint;
				FoundClimbableObject = true;
				ClimbHeight = i;
				if (DrawDebugVisuals) {
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, "Climb : Climbable object found");
				}
				ClimbStartLocation = HitResult.ImpactPoint;				
				break;
			}
			MaxSubsequentTraceForwardDistance = FVector::Distance(TraceStartVector, HitResult.ImpactPoint) + 30.f;
			LastHitLocation = HitResult.ImpactPoint;
			FoundClimbableObject = false;
		}
	} // first for loop ends	
	
	// If no climbable object found, return
	if (FoundClimbableObject == false) {
		// TODO: This could possible be a climb action
		TraceStartVector = FVector::ZeroVector;
		TraceEndVector = FVector::ZeroVector;
		if (DrawDebugVisuals) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Climb: No object found to climb on");
		}
		return;
	}

	bool ObstacleHit {false};
	FHitResult ObstacleHitResult {};
	
	// Climbable object found:
	// Start downward traces sequentially forward
	for (int i{0}; i < MaxDownwardTraces; i++) {
		
		TraceStartVector = (i == 0 ? (LastHitLocation + FVector(0, 0, TraceDownwardHeightStep)) : (TraceStartVector + (GetActorForwardVector()/*CameraForwardVector*/ * TraceDownwardOffsetDistance)));
		TraceEndVector = TraceStartVector - FVector(0, 0, MaxTraceDownwardDistance);

		// Perform a sweep from start vector to end vector
		HitSomething = World->SweepSingleByChannel(
			HitResult,
			TraceStartVector,
			TraceEndVector,
			FQuat::Identity,
			ECC_Visibility,
			FCollisionShape::MakeSphere(ForwardTraceSphereRadius)
		);
		
		// Check to see if continuing these traces will hit an obstacle (Do this only once, for performance)
		if (i == 0) {
			ObstacleHit = World->SweepSingleByChannel(
				ObstacleHitResult,
				TraceStartVector,
				(TraceStartVector + GetActorForwardVector()/*CameraForwardVector*/ * TraceDownwardOffsetDistance * MaxDownwardTraces),
				FQuat::Identity,
				ECC_Visibility,
				FCollisionShape::MakeSphere(20.f)
			);	
			if (ObstacleHit == true) {
				if (DrawDebugVisuals) {
					DrawDebugSphere(World, ObstacleHitResult.ImpactPoint, 20.f, 32, FColor::Red, false, 3.f);
				}
				if (DrawDebugVisuals) {
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, " >> Detected Obstacle");
				}
				if (HitSomething == true && ClimbHeight <= MaxMantleClimbHeight) {
					if (ClimbHeight <= MaxMantleClimbHeight1m) {														
						// Do a short climb (1 meter)																	// MANTLE 1m
						SelectedClimbMontage = ClimbAnimMontage1m;
						if (DrawDebugVisuals) {
							GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, " >> MANTLE: <= 1m");
						}
					} else {						
						// Do a long climb (2 meter)																	// MANTLE 2m
						SelectedClimbMontage = ClimbAnimMontage2m;
						if (DrawDebugVisuals) {
							GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, " >> MANTLE: > 1m");
						}
					}
					
					ClimbStartLocation = HitResult.ImpactPoint;
					ClimbEndLocation = ClimbStartLocation + (CameraForwardVector * LandingOffsetDistance);
					MantleMotionWarp();
					return;										
				} 				
			}
		}
		
		
		if (DrawDebugVisuals == true) {
			DrawDebugLine(World, TraceStartVector, TraceEndVector, FColor::Purple, false, 3.f);
			DrawDebugSphere(World, TraceStartVector, 15.f, 20, FColor::Green, false, 3.f);
			DrawDebugSphere(World, HitResult.ImpactPoint, 15.f, 20, FColor::Green, false, 3.f);
		}
		if (HitSomething == true) {
			if (i == 0) {
				// Set the position of the start of the climb
				ClimbStartLocation = HitResult.ImpactPoint;
				
			} else {
				// Set the position of the middle of the climb
				ClimbMiddleLocation = HitResult.ImpactPoint;
			}
			if (HitResult.GetActor()->ActorHasTag("Ground") && ClimbHeight <= MaxVaultClimbHeight) {					// VAULT
				// TODO: Handle Vaulting here...
				// Set the position of the end of the climb
				ClimbEndLocation = HitResult.Location + (GetActorForwardVector()/*CameraForwardVector*/ * LandingOffsetDistance);
				if (DrawDebugVisuals) {
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, " >> VAULT");
				}
				VaultMotionWarp();
				return;
			}
			
		}
	} // second for loop ends
	
	if (ClimbHeight <= MaxMantleClimbHeight) {
		if (ClimbHeight <= MaxMantleClimbHeight1m) {														
			// Do a short climb (1 meter)																				// MANTLE 1m
			SelectedClimbMontage = ClimbAnimMontage1m;
			if (DrawDebugVisuals) {
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, " >> MANTLE: <= 1m");
			}			
		} else {						
			// Do a long climb (2 meter)																				// MANTLE 2m
			SelectedClimbMontage = ClimbAnimMontage2m;
			if (DrawDebugVisuals) {
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, " >> MANTLE: > 1m");
			}
		}
					
		//ClimbStartLocation = HitResult.ImpactPoint;
		ClimbEndLocation = ClimbStartLocation + (CameraForwardVector * LandingOffsetDistance);
		MantleMotionWarp();
		return;	
	}
	
} // end of function


// Blueprint Native Events (C++ implementations)
void ARPG_PlayerCharacter::VaultMotionWarp_Implementation() {}
void ARPG_PlayerCharacter::MantleMotionWarp_Implementation() {}



// Protected Functions
/**
 * @brief Function that handles Player Attacks
 */
void ARPG_PlayerCharacter::Attack() {
	if (!PlayerMotionWarpingComponent || !PlayerAnimInstance) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Null references");
		return;
	}
	GetOverlappingActors(OverlappedActors);
	// Get the enemy to attack
	for (auto OverlappedActor : OverlappedActors) {
		if (OverlappedActor->Implements<UAssassination>()) {
			FEnemyReferencePosition EnemyReferencePosition = IAssassination::Execute_Stealth_BackstabAssassinate(OverlappedActor);
			// Enemy found
			PlayerAnimInstance->Montage_Play(StealthBackstabAttackMontage);	
			PlayerMotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(
				StealthBackstabWarpTargetName,
				EnemyReferencePosition.Position,
				EnemyReferencePosition.Rotation
			);
			return;
		}		
	}
	
}



// Private HUD Handling Functions
/**
 * @brief Update HUD Elements each frame (Call in Tick)
 * @param DeltaTime 
 */
void ARPG_PlayerCharacter::UpdatePlayerHUD_Tick(float DeltaTime) {
	if (PlayerHUDReference == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, "PlayerHUDReference is NULL!");
		return;
	}
	PlayerHUDReference->SetHealthBar_FillPercent(CurrentHealth/MaxHealth);
}

/**
 * @brief Update HUD Elements Periodically (Call on a Timer) 
 */
void ARPG_PlayerCharacter::UpdatePlayerHUD_OnTimer() {
	HandleStaminaDrainAndRefill();
}

// Private - Helper Functions
/**
 * @brief Called in BeginPlay() to initialize references
 */
void ARPG_PlayerCharacter::GetComponentReferences() {
	World = GetWorld();
	PlayerCapsuleComponent = GetCapsuleComponent();
	PlayerArrowComponent = GetArrowComponent();
	PlayerSkeletalMeshComponent = GetMesh();
	PlayerCharacterMovementComponent = GetCharacterMovement();
	if (PlayerSkeletalMeshComponent != nullptr) {
		PlayerAnimInstance = Cast<URPG_PlayerAnimInstance>(PlayerSkeletalMeshComponent->GetAnimInstance());
	}
}

/**
 * @brief Constructs HUD Elements and adds them to the Player's Viewport
 */
void ARPG_PlayerCharacter::AddHUDElementsToViewport() {
	auto PlayerHUD = CreateWidget(GetWorld(), PlayerHUDWidget, FName("Player HUD"));
	PlayerHUDReference = Cast<URPG_PlayerHUDWidget>(PlayerHUD);
	PlayerHUDReference->AddToViewport(0);
	// Initialize HUD
	InitiliazePlayerStaminaAndhealthBar();
}

/**
 * @brief Sets whether the player must use controller rotations, orient rottaion to movement etc.
 */
void ARPG_PlayerCharacter::ConfigurePlayerMovementAndRotationParams() {
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	if (PlayerCharacterMovementComponent != nullptr) {
		PlayerCharacterMovementComponent->bOrientRotationToMovement = true;
		PlayerCharacterMovementComponent->RotationRate = FRotator(0, 540.f, 0);
		PlayerCharacterMovementComponent->JumpZVelocity = 500.f;
		PlayerCharacterMovementComponent->AirControl = 0.2f;
	}
}

/**
 * @brief Handles running the stamina drain system on a timer
 */
void ARPG_PlayerCharacter::StartPlayerStatsUpdateTimer() {
	GetWorldTimerManager().SetTimer(
		StaminaUpdateTimerHandle,
		this,
		&ARPG_PlayerCharacter::UpdatePlayerHUD_OnTimer,
		StaminaUpdateTimeSeconds,
		true
	);
}

/**
 * @brief Sets the initial value of Player's Health and Stamina Bar (Call in BeginPlay)
 */
void ARPG_PlayerCharacter::InitiliazePlayerStaminaAndhealthBar() {
	PlayerHUDReference->SetStaminaBar_FillPercent(CurrentStamina/MaxStamina);
	PlayerHUDReference->SetHealthBar_FillPercent(CurrentHealth/MaxHealth);
}

/**
 * @brief Drains the Stamina on a Timer
 */
void ARPG_PlayerCharacter::HandleStaminaDrainAndRefill() {
	if (LogStaminaDrain == true) {
		UE_LOG(LogTemp, Display, TEXT(" >> Current Stamina = %f"), CurrentStamina);
	}	
	// Handle Stamina Drain and Refill
	if (bIsMoving && bIsSprinting) {
		// Drain Stamina [Player is Sprinting]
		CurrentStamina -= SprintingStaminaDrainRate;
		if (CurrentStamina <= MinStaminaForSprinting) {
			CanSprint = false;
			HandleWalk();
		}
		if (CurrentStamina <= 0.f) {
			CurrentStamina = 0.f;
		}
	} else {
		// Refill Stamina [Player is stationary]
		CurrentStamina += StaminaGainRate;
		if (CurrentStamina > MinStaminaForSprinting) {
			CanSprint = true;
		}
		if (CurrentStamina >= MaxStamina) {
			CurrentStamina = MaxStamina;
		}
	}
	// Update StaminaBar in HUD
	PlayerHUDReference->SetStaminaBar_FillPercent(CurrentStamina/MaxStamina);
}

/**
 * @brief Returns the Lateral Speed based on the supplied Velocity
 * @return 
 */
float ARPG_PlayerCharacter::GetCharacterLaterSpeed() {
	Velocity = GetVelocity();
	Velocity.Z = 0;
	return Velocity.Size();
}

/**
 * @brief Returns the Vertical Falling Speed of the Player
 * @return 
 */
float ARPG_PlayerCharacter::GetCharacterFallingSpeed() {
	Velocity = GetVelocity();
	Velocity.X = 0;
	Velocity.Y = 0;
	return Velocity.Size()/10;
}

/**
 * @brief Returns true if the Character is moving, based on Character's current velocity
 * @return 
 */
bool ARPG_PlayerCharacter::CheckIfCharacterIsMoving() {
	if (PlayerCharacterMovementComponent->GetCurrentAcceleration().Size() > 0.f) {
		return true;
	}
	return false;
}

void ARPG_PlayerCharacter::InterpolateMaxWalkSpeed(float DeltaTime) {
	if (PlayerCharacterMovementComponent == nullptr) {
		return;
	}
	PlayerCharacterMovementComponent->MaxWalkSpeed = FMath::FInterpTo(
		PlayerCharacterMovementComponent->MaxWalkSpeed,
		DesiredMaxWalkSpeed,
		DeltaTime,
		DesiredWalkSpeedInterpSpeed	
	);
}

void ARPG_PlayerCharacter::InterpolateSpringArmLength(float DeltaTime) {
	if (PlayerSpringArmComponent == nullptr) {
		return;
	}
	PlayerSpringArmComponent->TargetArmLength = FMath::FInterpTo(
		PlayerSpringArmComponent->TargetArmLength,
		DesiredTargetArmLength,
		DeltaTime,
		TargetArmLengthInterpSpeed
	);
}
