
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPG_PlayerHUDWidget.generated.h"


UCLASS()
class THIRDPERSON_RPG_API URPG_PlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY()
	class ARPG_PlayerCharacter* PlayerCharacter {nullptr};
	
protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UCanvasPanel* CanvasPanel {nullptr};
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UProgressBar* HealthBar {nullptr};
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UProgressBar* StaminaBar {nullptr};

public:
	// Public Setters:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetHealthBar_FillPercent(float percent);
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetStaminaBar_FillPercent(float percent);
	
};
