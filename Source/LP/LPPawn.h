#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LPPawn.generated.h"

UCLASS()
class LP_API ALPPawn : public APawn
{
	GENERATED_BODY()

public:
	ALPPawn();

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Default)
	class UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, Category = Default)
	class ULockPickingComponent* LockPicking;
	UPROPERTY(EditAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* LockMesh;

private:
	void RotateLock();
	void ReleaseLock();
	void RotateLockPick(float Value);
	void ShowDebug();
	template<int32 Index> void ChangeLevel();
};
