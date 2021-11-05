// Cameron Davis, 2021. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Math/Vector.h"
#include "SnakeGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RETROREMAKES_API ASnakeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Snake Game Mode")
	void UpdateScore(uint8 Value);

	UPROPERTY(EditAnywhere, Category = "Snake Game Mode")
	TSubclassOf<AActor> FoodClass;	
	UPROPERTY(EditAnywhere, Category = "Snake Game Mode");
	TSubclassOf<AActor> SnakeBodyClass;
	UPROPERTY(EditAnywhere, Category = "Snake Game Mode")
	TSubclassOf<UUserWidget> GameHUDClass;
	UPROPERTY(EditAnywhere, Category = "Snake Game Mode")
	UPaperFlipbook* HeadFlipbook;
	UPROPERTY(EditAnywhere, Category = "Snake Game Mode")
	UPaperFlipbook* BodyFlipbook;

	UPROPERTY(BlueprintReadOnly, Category = "Snake Game Mode")
	UUserWidget* GameHUDWidget;
	UPROPERTY(EditAnywhere, Category = "Snake Game Mode")
	float UpdateDelay;
protected:
	virtual void BeginPlay() override;
private:
	UFUNCTION()
	void SpawnNewSnakePart(FVector SpawnLocation, bool IsHead);
	UFUNCTION()
	void SpawnFood();
	UFUNCTION()
	void KillSnake();
	UFUNCTION()
	void SelectRandomDirection();
	
	TArray<AActor*> SnakePartsArray;
	UPaperFlipbookComponent* FlipbookComponent;
	bool IsDead;
	bool CanChangeDirection;
	uint8 Score = 0;
	uint8 DirectionZ;
	uint8 DirectionX;
	float Z_Max = 300;
	float X_Max = 300;
	
};
