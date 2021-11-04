// Cameron Davis, 2021. All rights reserved.


#include "SnakeLevelScriptActor.h"
#include "Kismet/GameplayStatics.h"

void ASnakeLevelScriptActor::SetViewTarget()
{
	// Get the local player controller
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// Set the player view
	PlayerController->SetViewTargetWithBlend(CameraActor, 0.f);
}
