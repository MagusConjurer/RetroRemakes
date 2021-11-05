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
	case EFoodColor::FC_Green:
		FoodSpriteComponent->SetSpriteColor(FLinearColor(40.f, 60.f, 30.f));
		Score = 10;
		TimerDelegate.BindLambda([&] {SetFood(EFoodColor::FC_Yellow); });
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 3.f, false);
		break;
	case EFoodColor::FC_Yellow:
		FoodSpriteComponent->SetSpriteColor(FLinearColor(140.f, 140.f, 90.f));
		Score = 20;
		TimerDelegate.BindLambda([&] {SetFood(EFoodColor::FC_Orange); });
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 3.f, false);
		break;
	case EFoodColor::FC_Orange:
		FoodSpriteComponent->SetSpriteColor(FLinearColor(130.f, 85.f, 30.f));
		Score = 50;
		TimerDelegate.BindLambda([&] {SetFood(EFoodColor::FC_Red); });
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 3.f, false);
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

