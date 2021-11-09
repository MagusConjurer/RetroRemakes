// Cameron Davis, 2021. All rights reserved.

#include "PaperSpriteComponent.h"
#include "FoodActor.h"

// Sets default values
AFoodActor::AFoodActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FoodSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Food Sprite"));
	RootComponent = FoodSpriteComponent;
}

// Called when the game starts or when spawned
void AFoodActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFoodActor::SetFood(EFoodColor FCEnum)
{
	FoodSpriteComponent->SetSprite(FoodSprite);
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	
	switch (FCEnum)
	{
	case EFoodColor::FC_Red:
		FoodSpriteComponent->SetSpriteColor(FLinearColor(0.5f, 0.0f, 0.0f));
		Score = 100;
		TimerDelegate.BindLambda([&] {SetFood(EFoodColor::FC_Orange); });
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, FoodTimerDelay, false);
		break;
	case EFoodColor::FC_Orange:
		FoodSpriteComponent->SetSpriteColor(FLinearColor(0.75f, 0.25f, 0.0f));
		Score = 50;
		TimerDelegate.BindLambda([&] {SetFood(EFoodColor::FC_Yellow); });
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, FoodTimerDelay, false);
		break;
	case EFoodColor::FC_Yellow:
		FoodSpriteComponent->SetSpriteColor(FLinearColor(1.0f, 1.0f, 0.0f));
		Score = 20;
		TimerDelegate.BindLambda([&] {SetFood(EFoodColor::FC_Green); });
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, FoodTimerDelay, false);
		break;
	case EFoodColor::FC_Green:
		FoodSpriteComponent->SetSpriteColor(FLinearColor(0.0f, 0.35f, 0.0f));
		Score = 10;
		break;
	}
}

// Called every frame
void AFoodActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

