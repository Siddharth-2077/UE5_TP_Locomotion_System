
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RPG_PlayerAnimInstance.generated.h"


UCLASS()
class THIRDPERSON_RPG_API URPG_PlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	
private:
	// Reference to the Player Character
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	class ARPG_PlayerCharacter* PlayerCharacter {nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	class ARPG_PlayerController* PlayerCharacterController {nullptr};

#pragma region // Animation Parameters

	/**
	 * @brief The Lateral Speed of the RPGCharacter
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player State", meta = (AllowPrivateAccess = "true"))
	float Speed {0.f};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player State", meta = (AllowPrivateAccess = "true"))
	float VerticalAxisValue {0.f};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player State", meta = (AllowPrivateAccess = "true"))
	float HorizontalAxisValue {0.f};

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

	FVector Velocity {};
	
#pragma endregion
	
	
};
