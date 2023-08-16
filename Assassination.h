
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Assassination.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAssassination : public UInterface
{
	GENERATED_BODY()
};



// Struct that will contain the enemy's target position and rotation
USTRUCT(BlueprintType)
struct FEnemyReferencePosition {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Position;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation;
};


class THIRDPERSON_RPG_API IAssassination {
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FEnemyReferencePosition Stealth_BackstabAssassinate();
	
};
