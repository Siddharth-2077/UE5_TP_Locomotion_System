
#pragma once

#include "CoreMinimal.h"
#include "Assassination.h"
#include "Health.h"
#include "GameFramework/Character.h"
#include "RPG_PlayerCharacter.generated.h"


// The Player class
UCLASS()
class THIRDPERSON_RPG_API ARPG_PlayerCharacter : public ACharacter, public IHealth, public IAssassination
{
	
	GENERATED_BODY()

public:	
	ARPG_PlayerCharacter();
	
protected:	
	virtual void BeginPlay() override;
	
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region // IHealth - Interface Overrides

	virtual void IncreaseHealth_Implementation(float Health) override;
	virtual void DecreaseHealth_Implementation(float Health) override;
	virtual void CharacterDead_Implementation() override;
	
#pragma endregion 

#pragma region // IAssassination - Interface Overrides
	virtual FEnemyReferencePosition Stealth_BackstabAssassinate_Implementation() override;
#pragma endregion 

private:

#pragma region // Component References

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class UWorld* World {nullptr};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class UCapsuleComponent* PlayerCapsuleComponent {nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class UArrowComponent* PlayerArrowComponent {nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class USkeletalMeshComponent* PlayerSkeletalMeshComponent {nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class UCharacterMovementComponent* PlayerCharacterMovementComponent {nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class URPG_PlayerAnimInstance* PlayerAnimInstance {nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class USpringArmComponent* PlayerSpringArmComponent {nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* PlayerCameraComponent {nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class UMotionWarpingComponent* PlayerMotionWarpingComponent {nullptr};
	
	
#pragma endregion

#pragma region // Movement Parameters
	FVector Velocity {};
	
	/**
	 * @brief The Lateral Speed of the RPGCharacter
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player State", meta = (AllowPrivateAccess = "true"))
	float Speed {0.f};
	/**
	 * @brief The Vertical Speed of the player while Falling
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player State", meta = (AllowPrivateAccess = "true"))
	float FallingSpeed {0.f};
	/**
	 * @brief Is the RPGCharacter in Air (Jumping or Falling)
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player State", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir {false};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player State", meta = (AllowPrivateAccess = "true"))
	bool bIsFalling {false};	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player State", meta = (AllowPrivateAccess = "true"))
	bool bIsMoving {false};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player State", meta = (AllowPrivateAccess = "true"))
	bool bIsCrouching {false};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player State", meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting {false};

	UPROPERTY()
	bool CanSprint {true};
	UPROPERTY()
	float DesiredMaxWalkSpeed {0.f};	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float WalkSpeed {200.f};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float SprintSpeed {500.f};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float CrouchedSpeed {200.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DesiredWalkSpeedInterpSpeed {10.f};

	// >> Climbing Properties:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Climb Parameters", meta = (AllowPrivateAccess = "true"))
	bool bIsClimbing {false};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Climb Parameters", meta = (AllowPrivateAccess = "true"))
	int MaxVaultClimbHeight {5};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Climb Parameters", meta = (AllowPrivateAccess = "true"))
	int MaxMantleClimbHeight {8};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Climb Parameters", meta = (AllowPrivateAccess = "true"))
	int MaxMantleClimbHeight1m {5};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Climb Parameters", meta = (AllowPrivateAccess = "true"))
	int MaxMantleClimbHeight2m {8};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Climb Parameters|Vault", meta = (AllowPrivateAccess = "true"))
	float LandingOffsetDistance {50.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Climb Parameters|Vault", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* VaultAnimMontage {nullptr};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Climb Parameters|Climb", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* ClimbAnimMontage1m {nullptr};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Climb Parameters|Climb", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* ClimbAnimMontage2m {nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Climb Parameters|Climb", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* SelectedClimbMontage {nullptr};
	
	UPROPERTY()
	FVector TraceStartVector {FVector::ZeroVector};
	UPROPERTY()
	FVector TraceEndVector {FVector::ZeroVector};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Climb Parameters|Trace", meta = (AllowPrivateAccess = "true"))
	bool DrawDebugVisuals {false};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Climb Parameters|Trace|Forward Trace", meta = (AllowPrivateAccess = "true"))
	float MaxTraceForwardDistance {200.f};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Climb Parameters|Trace|Forward Trace", meta = (AllowPrivateAccess = "true"))
	float MaxSubsequentTraceForwardDistance {0.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Climb Parameters|Trace|Forward Trace", meta = (AllowPrivateAccess = "true"))
	float ForwardTraceHeightOffset {30.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Climb Parameters|Trace|Forward Trace", meta = (AllowPrivateAccess = "true"))
	int MaxUpwardTraces {10};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Climb Parameters|Trace|Forward Trace", meta = (AllowPrivateAccess = "true", UIMin="3", UIMax="15"))
	float ForwardTraceSphereRadius {7.5f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Climb Parameters|Trace|Downward Trace", meta = (AllowPrivateAccess = "true"))
	float TraceDownwardHeightStep {50.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Climb Parameters|Trace|Downward Trace", meta = (AllowPrivateAccess = "true"))
	float TraceDownwardOffsetDistance {30.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Climb Parameters|Trace|Downward Trace", meta = (AllowPrivateAccess = "true"))
	float MaxTraceDownwardDistance {100.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Climb Parameters|Trace|Downward Trace", meta = (AllowPrivateAccess = "true"))
	int MaxDownwardTraces {6};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FVector ClimbStartLocation {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FVector ClimbMiddleLocation {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FVector ClimbEndLocation {};
	
#pragma endregion

#pragma region // Camera Properties
	UPROPERTY()
	float DesiredTargetArmLength {0.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float NormalTargetArmLength {200.f};	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float CrouchedTargetArmLength {350.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float TargetArmLengthInterpSpeed {2.f};
#pragma endregion

#pragma region // Stamina and Health Properties

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Stats|Health", meta=(AllowPrivateAccess="true"))
	float MaxHealth{100.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Stats|Health", meta=(AllowPrivateAccess="true"))
	float CurrentHealth{80.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Stats|Stamina", meta=(AllowPrivateAccess="true"))
	float MaxStamina{100.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Stats|Stamina", meta=(AllowPrivateAccess="true"))
	float CurrentStamina{100.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Stats|Stamina", meta=(AllowPrivateAccess="true"))
	float MinStaminaForSprinting {15.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Stats|Stamina|Gain", meta=(AllowPrivateAccess="true"))
	float StaminaGainRate{2.f};
	UPROPERTY()
	FTimerHandle StaminaUpdateTimerHandle {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Stats|Stamina|Drain", meta=(AllowPrivateAccess="true"))
	bool LogStaminaDrain {false};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Stats|Stamina|Drain", meta=(AllowPrivateAccess="true"))
	float StaminaUpdateTimeSeconds{0.25f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Stats|Stamina|Drain", meta=(AllowPrivateAccess="true"))
	float SprintingStaminaDrainRate {1.f};
	// TODO: Add more stamina drain params for various actions
	
#pragma endregion 
	
#pragma region // HUD Properties

	// Player HUD
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HUD", meta=(AllowPrivateAccess="true"))					// TODO: FIX REFERENCES
	TSubclassOf<class URPG_PlayerHUDWidget> PlayerHUDWidget;
	UPROPERTY()
	class URPG_PlayerHUDWidget* PlayerHUDReference {nullptr};
	
#pragma endregion

#pragma region // Attacking Properties
	
	UPROPERTY()
	TArray<AActor*> OverlappedActors {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Stealth Backstab", meta=(AllowPrivateAccess="true"))
	FName StealthBackstabWarpTargetName {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack|Stealth Backstab", meta=(AllowPrivateAccess="true"))
	class UAnimMontage* StealthBackstabAttackMontage {nullptr};
	
#pragma endregion 

	
public:	
	void UpdatePlayerProperties();
	
#pragma region // Public Getters

	UFUNCTION(BlueprintCallable, Category="RPG Player")
	FORCEINLINE	UCapsuleComponent* GetPlayerCapsuleComponent() const {return PlayerCapsuleComponent;}
	UFUNCTION(BlueprintCallable, Category="RPG Player")
	FORCEINLINE	UArrowComponent* GetPlayerArrowComponent() const {return PlayerArrowComponent;}
	UFUNCTION(BlueprintCallable, Category="RPG Player")
	FORCEINLINE	USkeletalMeshComponent* GetPlayerSkeletalMeshComponent() const {return PlayerSkeletalMeshComponent;}
	UFUNCTION(BlueprintCallable, Category="RPG Player")
	FORCEINLINE	UCharacterMovementComponent* GetPlayerCharacterMovementComponent() const {return PlayerCharacterMovementComponent;}
	UFUNCTION(BlueprintCallable, Category="RPG Player")
	FORCEINLINE	URPG_PlayerAnimInstance* GetPlayerAnimInstance() const {return PlayerAnimInstance;}
	UFUNCTION(BlueprintCallable, Category="RPG Player")
	FORCEINLINE	USpringArmComponent* GetPlayerSpringArmComponent() const {return PlayerSpringArmComponent;}
	UFUNCTION(BlueprintCallable, Category="RPG Player")
	FORCEINLINE	UCameraComponent* GetPlayerCameraComponent() const {return PlayerCameraComponent;}
	UFUNCTION(BlueprintCallable, Category="RPG Player")
	FORCEINLINE	UMotionWarpingComponent* GetPlayerMotionWarpingComponent() const {return PlayerMotionWarpingComponent;}

	UFUNCTION()
	float GetPlayerSpeed() const {return Speed;}
	UFUNCTION()
	bool GetPlayerIsInAir() const {return bIsInAir;}
	UFUNCTION()
	bool GetPlayerIsMoving() const {return bIsMoving;}
	UFUNCTION()
	bool GetPlayerIsCrouching() const {return bIsCrouching;}
	UFUNCTION()
	bool GetPlayerIsFalling() const {return bIsFalling;}
	UFUNCTION()
	float GetPlayerFallingSpeed() const {return FallingSpeed;}
	
	
#pragma endregion 

#pragma region // Public Setters

#pragma endregion
	
#pragma region // Movement Handling Functions

	UFUNCTION()
	void HandleCrouch();
	UFUNCTION()
	void HandleWalk();
	UFUNCTION()
	void ToggleSprinting();	
	UFUNCTION()
	void ToggleCrouching();
	UFUNCTION()
	void HandleClimb();

	UFUNCTION(BlueprintNativeEvent)
	void VaultMotionWarp();
	UFUNCTION(BlueprintNativeEvent)
	void MantleMotionWarp();
	
#pragma endregion 
	
#pragma region // Attack Handling Functions

	UFUNCTION()
	void Attack();
	
#pragma endregion 

	
private:

#pragma region // HUD Updating Functions
	void UpdatePlayerHUD_Tick(float DeltaTime);
	void UpdatePlayerHUD_OnTimer();
#pragma endregion

#pragma region // Player Stats Updating Functions
	void StartPlayerStatsUpdateTimer();
	void InitiliazePlayerStaminaAndhealthBar();	
	void HandleStaminaDrainAndRefill();
#pragma endregion 
	
#pragma region // Private Functions
	void GetComponentReferences();
	void AddHUDElementsToViewport();
	void ConfigurePlayerMovementAndRotationParams();
	
	float GetCharacterLaterSpeed();
	float GetCharacterFallingSpeed();
	bool CheckIfCharacterIsMoving();

	void InterpolateMaxWalkSpeed(float DeltaTime);
	void InterpolateSpringArmLength(float DeltaTime);

	void SetMaxWalkSpeed(float speed) {DesiredMaxWalkSpeed = speed;};
#pragma endregion
	
};
