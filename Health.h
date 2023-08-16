
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Health.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHealth : public UInterface {
	GENERATED_BODY()
};


class THIRDPERSON_RPG_API IHealth {
	GENERATED_BODY()
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Functions:	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void IncreaseHealth(float Health);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DecreaseHealth(float Health);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CharacterDead();
	
};
