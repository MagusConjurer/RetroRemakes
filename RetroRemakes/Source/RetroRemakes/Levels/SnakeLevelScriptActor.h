// Cameron Davis, 2021. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "SnakeLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class RETROREMAKES_API ASnakeLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	AActor* CameraActor;
private:
protected:
	UFUNCTION(BlueprintCallable, Category = LevelBlueprint)
	void SetViewTarget();
	
};
