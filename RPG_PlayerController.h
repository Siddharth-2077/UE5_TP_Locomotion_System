
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPG_PlayerController.generated.h"


UCLASS()
class THIRDPERSON_RPG_API ARPG_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	
#pragma region // Virtual Function overrides
	
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;
	
#pragma endregion 		
	
private:
	UPROPERTY()
	class ARPG_PlayerCharacter* PlayerCharacter {nullptr};																// Player Reference
	
#pragma region // Input Handling Functions

	UFUNCTION()
	void SpaceBarPressedHandler();
	UFUNCTION()
	void LeftShiftPressedHandler();
	UFUNCTION()
	void LeftControlPressedHandler();
	UFUNCTION()
	void AttackPressedHandler();
	UFUNCTION()
	void VerticalAxisHandler(float Value);
	UFUNCTION()
	void HorizontalAxisHandler(float Value);
	UFUNCTION()
	void MouseXHandler(float Value);
	UFUNCTION()
	void MouseYHandler(float Value);
	
#pragma endregion

#pragma region // Player Movement Functions

	UFUNCTION()
	void MoveForward(float VerticalAxisValue);
	UFUNCTION()
	void MoveSideways(float HorizontalAxisValue);
	
#pragma endregion
	
};
