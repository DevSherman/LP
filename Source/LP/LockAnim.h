#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LockAnim.generated.h"


UCLASS()
class LP_API ULockAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRotation(float Value);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateLockpickRotation(float Value);
};
