
#include "RPG_PlayerHUDWidget.h"

#include "RPG_PlayerCharacter.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

// Native Construct
void URPG_PlayerHUDWidget::NativeConstruct() {
	Super::NativeConstruct();	
}

// Native Initialized
void URPG_PlayerHUDWidget::NativeOnInitialized() {
	Super::NativeOnInitialized();
	// Get Reference to the Player
	PlayerCharacter = Cast<ARPG_PlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}


// Public Setters:
/**
 * @brief Sets the fill percent of the Health-Bar
 * @param percent 
 */
void URPG_PlayerHUDWidget::SetHealthBar_FillPercent(float percent) {
	if (HealthBar == nullptr) {
		return;
	}
	HealthBar->SetPercent(percent);
}

/**
 * @brief Sets the fill percent of the Stamina-Bar
 * @param percent 
 */
void URPG_PlayerHUDWidget::SetStaminaBar_FillPercent(float percent) {
	if (StaminaBar == nullptr) {
		return;
	}
	StaminaBar->SetPercent(percent);
}




