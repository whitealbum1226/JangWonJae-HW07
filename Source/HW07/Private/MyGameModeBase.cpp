#include "MyGameModeBase.h"
#include "MyPawn.h"

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = AMyPawn::StaticClass();
}
