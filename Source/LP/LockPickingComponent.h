#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LockPickingComponent.generated.h"

UENUM()
enum ELockPickingLevel
{
	EASY = 0 UMETA(DisplayName = "Easy"),
	NORMAL = 1 UMETA(DisplayName = "Normal"),
	HARD = 2 UMETA(DisplayName = "Hard"),
	VERY_HARD = 3 UMETA(DisplayName = "Very Hard"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LP_API ULockPickingComponent : public USceneComponent
{
	GENERATED_BODY()
	ULockPickingComponent();

public:
	void Update(float DeltaTime);
	void SetAnim(class UAnimInstance* AnimInstance);
	void Generate(ELockPickingLevel _Level = ELockPickingLevel::EASY);
	FString GetLevelName();
	ELockPickingLevel GetLevel() { return Level; }

	//Compare lockpickrotation  to get a max rotation (near or far, to range)
	float GetMaxRotation();

	void RotateLock();
	void ReleaseLock();
	void ShowDebug();
	void RotateLockPick(float Value);
	void ChangeLevel(int32 Index);

protected:
	UPROPERTY(EditAnywhere, Category = "Lockpicking Level")
	TEnumAsByte<ELockPickingLevel> Level;
	UPROPERTY(EditAnywhere, Category = "Lockpicking Level")
	int32 Easy_Areas = 6;
	UPROPERTY(EditAnywhere, Category = "Lockpicking Level")
	int32 Normal_Areas = 8;
	UPROPERTY(EditAnywhere, Category = "Lockpicking Level")
	int32 Hard_Areas = 10;
	UPROPERTY(EditAnywhere, Category = "Lockpicking Level")
	int32 Very_Hard_Areas = 12;

	UPROPERTY(EditAnywhere, Category = "Lockpicking Attempts")
	int32 MaxLockpickDurability = 6;
	UPROPERTY(EditAnywhere, Category = "Lockpicking Attempts")
	float BreakTimeMax = 1.5f;


	//lock rotation, default: 90 degrees 1/4
	UPROPERTY(EditAnywhere, Category = "Lockpicking Rotation")
	float MinRotation = 0;
	UPROPERTY(EditAnywhere, Category = "Lockpicking Rotation")
	float MaxRotation = 90;
	//lockpick rotation, default: 180 degrees 1/2
	UPROPERTY(EditAnywhere, Category = "Lockpicking Rotation")
	float MinLockpickRotation = 0;
	UPROPERTY(EditAnywhere, Category = "Lockpicking Rotation")
	float MaxLockpickRotation = 180;
	UPROPERTY(EditAnywhere, Category = "Lockpicking Rotation")
	float FixAnimLockpickRotation = -90;

	UPROPERTY(EditAnywhere, Category = "Default")
	bool bDEBUG = false;

	class ULockAnim* Anim;
	UPROPERTY(EditAnywhere, Category = "Lockpicking Rotation")
	float RotationAmount = 0.7f;
	UPROPERTY(EditAnywhere, Category = "Lockpicking Rotation")
	float LockPickRotationAmount = 2.5f;
	UPROPERTY(EditAnywhere, Category = "Lockpicking Rotation")
	float MarginError = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Lockpicking Rotation")
	class UAudioComponent* Audio;
	UPROPERTY(EditAnywhere, Category = "Lockpicking Rotation")
	class USoundBase* TurnLockSound;

private:
	float TargetAngleMin = 0;
	float TargetAngleMax = 0;
	bool  bTurning = false;
	float RotationValue = 0; //lock rotation
	float LockPickCurrentValue = 90; //current lockpick rotation (up:90) (0 to 180)
	float LockPickTargetValue = 0;  //target lockpick rotation
	//aux values (no clampled)
	float _LockPickCurrentValue = 0;
	bool bSucess = false;
	//per level
	int32 LockpickDurability = 0;
	float BreakTimeCurrent = 0;

private:
	bool CheckResult();
	bool InRange();
	int GetSplitValue();
};
