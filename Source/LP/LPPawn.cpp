#include "LPPawn.h"
#include "Camera/CameraComponent.h"
#include "LockPickingComponent.h"

ALPPawn::ALPPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	RootComponent = Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	LockMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LockMesh"));
	LockPicking = CreateDefaultSubobject<ULockPickingComponent>(TEXT("LockPicking"));
}

void ALPPawn::BeginPlay()
{
	Super::BeginPlay();
	LockPicking->SetAnim(LockMesh->GetAnimInstance());
	LockPicking->Generate();
}

void ALPPawn::RotateLock() { LockPicking->RotateLock(); }
void ALPPawn::ReleaseLock() { LockPicking->ReleaseLock(); }
void ALPPawn::ShowDebug() { LockPicking->ShowDebug(); }
void ALPPawn::RotateLockPick(float Value) { LockPicking->RotateLockPick(Value); }
template<int32 Index> void ALPPawn::ChangeLevel() { LockPicking->ChangeLevel(Index); }

void ALPPawn::Tick(float DeltaTime)
{
	LockPicking->Update(DeltaTime);
}

void ALPPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("RotateLock", IE_Pressed, this, &ALPPawn::RotateLock);
	PlayerInputComponent->BindAction("RotateLock", IE_Released, this, &ALPPawn::ReleaseLock);
	PlayerInputComponent->BindAxis("HMouse", this, &ALPPawn::RotateLockPick);

	PlayerInputComponent->BindAction("Help", IE_Pressed, this, &ALPPawn::ShowDebug);

	PlayerInputComponent->BindAction("Num1", IE_Pressed, this, &ALPPawn::ChangeLevel<0>);
	PlayerInputComponent->BindAction("Num2", IE_Pressed, this, &ALPPawn::ChangeLevel<1>);
	PlayerInputComponent->BindAction("Num3", IE_Pressed, this, &ALPPawn::ChangeLevel<2>);
	PlayerInputComponent->BindAction("Num4", IE_Pressed, this, &ALPPawn::ChangeLevel<3>);
}

