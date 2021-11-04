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
	switch (FCEnum)
	{
	case EFoodColor::FC_Green:
		FoodSpriteComponent->SetSpriteColor(FLinearColor(40.f, 60.f, 30.f));
		Score = 10;
		GetWorldTimerManager().SetTimer(TimerHandle, 3.f, false);
		SetFood(EFoodColor::FC_Yellow);
		break;
	case EFoodColor::FC_Yellow:
		FoodSpriteComponent->SetSpriteColor(FLinearColor(140.f, 140.f, 90.f));
		Score = 20;
		GetWorldTimerManager().SetTimer(TimerHandle, 2.f, false);
		SetFood(EFoodColor::FC_Orange);
		break;
	case EFoodColor::FC_Orange:
		FoodSpriteComponent->SetSpriteColor(FLinearColor(130.f, 85.f, 30.f));
		Score = 50;
		GetWorldTimerManager().SetTimer(TimerHandle, 1.f, false);
		SetFood(EFoodColor::FC_Red);
		break;
	case EFoodColor::FC_Red:
		FoodSpriteComponent->SetSpriteColor(FLinearColor(130.f, 40.f, 30.f));
		Score = 100;
		break;
	}
}

// Called every frame
void AFoodActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

