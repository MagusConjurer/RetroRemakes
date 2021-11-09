// Cameron Davis, 2021. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "FoodActor.generated.h"

UENUM(BlueprintType)
enum class EFoodColor : uint8 {
	FC_Red UMETA(DisplayName = "Red"),
	FC_Orange UMETA(DisplayName = "Orange"),
	FC_Yellow UMETA(DisplayName = "Yellow"),
	FC_Green UMETA(DisplayName = "Green")
};

UCLASS()
class RETROREMAKES_API AFoodActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFoodActor();

	UPROPERTY(EditAnywhere)
	float FoodTimerDelay = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFoodColor FoodColorEnum;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	uint8 Score;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Update the food based on the current status of the enum
	UFUNCTION(BlueprintCallable, Category = FoodActor)
	virtual void SetFood(EFoodColor FCEnum);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	UPaperSprite* FoodSprite;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPaperSpriteComponent* FoodSpriteComponent;
};
