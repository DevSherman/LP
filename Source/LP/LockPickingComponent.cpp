#include "LockPickingComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "LockAnim.h"
#include <Kismet/GameplayStatics.h>

ULockPickingComponent::ULockPickingComponent()
{
	Audio = CreateDefaultSubobject<UAudioComponent>("Audio");
}

void ULockPickingComponent::SetAnim(UAnimInstance* AnimInstance)
{
	Anim = Cast<ULockAnim>(AnimInstance);
}

void ULockPickingComponent::Generate(ELockPickingLevel _Level)
{
	//set level
	Level = _Level;
	//many times to split a angle between 0 to 180
	int SplitValue = GetSplitValue();
	//partial angle //example: EASY 30 degrees
	float Angle = 180.0f / SplitValue;
	//select randomly a sector (min, max)
	TargetAngleMin = Angle * FMath::RandRange(1, SplitValue - 1);
	TargetAngleMax = TargetAngleMin + Angle;

	_LockPickCurrentValue = LockPickCurrentValue;
	bSucess = false;


	int32 _LevelIndex = static_cast<uint8>(Level);
	LockpickDurability = MaxLockpickDurability - _LevelIndex;

	if (LockpickDurability <= 0) LockpickDurability = MaxLockpickDurability;

	Audio->SetSound(TurnLockSound);
	Audio->Play();
	Audio->SetPaused(true);
}

void ULockPickingComponent::Update(float DeltaTime)
{
	if (Audio->IsPlaying())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("Playing.")));
	}
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("Audio Name: %s"), *Audio->Sound->GetName()));

	if (bSucess)
	{
		//Exit
		if (bDEBUG && GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("SUCESS, Select a level to reset.")));
		return;
	}

	if (LockpickDurability > 0)
	{
		if (RotationValue >= GetMaxRotation())
		{
			BreakTimeCurrent += DeltaTime;
			if (BreakTimeCurrent > BreakTimeMax)
			{
				LockpickDurability--;
				BreakTimeCurrent = 0;
			}
			Audio->SetPaused(true);
		}
		else
		{
			Audio->SetPaused(false);
		}
	}
	else
	{
		//break
		if (bDEBUG && GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("LOCKPICK BREAKED, Select a level to reset.")));
		return;
	}

	//update rotation
	if (bTurning)
	{
		RotationValue += RotationAmount;
	}
	else
	{
		RotationValue -= RotationAmount;
	}

	RotationValue = FMath::Clamp<float>(RotationValue, MinRotation, GetMaxRotation());
	Anim->UpdateRotation(RotationValue);

	bSucess = CheckResult();

	if (bDEBUG && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("MaxRotation: %f"), GetMaxRotation()));
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("RotationValue: %f"), RotationValue));
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("LockPickCurrentValue: %f"), LockPickCurrentValue));
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("TargetValueMin: %f TargetValueMax: %f"), TargetAngleMin, TargetAngleMax));
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("LockpickDurability: %d"), LockpickDurability));

		if (InRange()) GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("InRange")));
	}
}

void ULockPickingComponent::RotateLockPick(float Value)
{
	if (bSucess) return;

	_LockPickCurrentValue += Value * LockPickRotationAmount;
	LockPickCurrentValue = FMath::Clamp<float>(_LockPickCurrentValue, MinLockpickRotation, MaxLockpickRotation);
	//fixed rotation (-90 to 90 degrees)
	Anim->UpdateLockpickRotation(LockPickCurrentValue + FixAnimLockpickRotation);
}

bool ULockPickingComponent::CheckResult()
{
	return RotationValue >= MaxRotation - MarginError;
}

bool ULockPickingComponent::InRange()
{
	return (LockPickCurrentValue >= TargetAngleMin && LockPickCurrentValue <= TargetAngleMax);
}

float ULockPickingComponent::GetMaxRotation()
{
	if (InRange()) return MaxRotation;
	else
	{
		float result;
		if (LockPickCurrentValue > TargetAngleMax) result = LockPickCurrentValue - TargetAngleMax;
		else if(LockPickCurrentValue < TargetAngleMin) result = TargetAngleMin - LockPickCurrentValue;

		if (MaxRotation - result > 0) return MaxRotation - result;
		else return 0.5f;
	}
}

int ULockPickingComponent::GetSplitValue()
{
	switch (Level)
	{
		case ELockPickingLevel::EASY: return Easy_Areas; break;
		case ELockPickingLevel::NORMAL: return Normal_Areas; break;
		case ELockPickingLevel::HARD: return Hard_Areas; break;
		case ELockPickingLevel::VERY_HARD: return Very_Hard_Areas; break;

		default: return -1; break;
	}
}

void ULockPickingComponent::RotateLock()
{
	bTurning = true;
}
void ULockPickingComponent::ReleaseLock()
{
	bTurning = false;
}

void ULockPickingComponent::ShowDebug()
{
	bDEBUG = !bDEBUG;
}

void ULockPickingComponent::ChangeLevel(int32 Index)
{
	Generate(static_cast<ELockPickingLevel>(Index));
}

FString ULockPickingComponent::GetLevelName()
{
	switch (Level)
	{
		case ELockPickingLevel::EASY: return "Easy"; break;
		case ELockPickingLevel::NORMAL: return "Normal"; break;
		case ELockPickingLevel::HARD: return "Hard"; break;
		case ELockPickingLevel::VERY_HARD: return "Very Hard"; break;

		default: return ""; break;
	}
}

