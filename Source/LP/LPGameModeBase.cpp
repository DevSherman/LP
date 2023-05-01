#include "LPGameModeBase.h"
#include "LPPawn.h"

ALPGameModeBase::ALPGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/BP_LPPawn"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}