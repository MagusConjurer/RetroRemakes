// Cameron Davis, 2021. All rights reserved.


#include "SnakeGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Math/UnrealMathUtility.h"

void ASnakeGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GameHUDClass))
	{
		GameHUDWidget = CreateWidget(GetWorld(), GameHUDClass);
		if (GameHUDWidget != nullptr)
		{
			GameHUDWidget->AddToViewport();
		}
	}

	IsDead = false;
}

void ASnakeGameModeBase::SpawnNewSnakePart(FVector SpawnLocation, bool IsHead)
{
	// Spawn a SnakeBody actor at the given location while ignoring collision
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* SnakeBodyActor = GetWorld()->SpawnActor(SnakeBodyClass, &SpawnLocation, &FRotator::ZeroRotator, SpawnParams);

	if (SnakeBodyActor != nullptr)
	{
		if (IsHead && IsValid(HeadFlipbook))
		{
			FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook Component"));
			FlipbookComponent->SetFlipbook(HeadFlipbook);
			SnakeBodyActor->SetRootComponent(FlipbookComponent);
		}
		if (!IsHead && IsValid(BodyFlipbook))
		{
			FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook Component"));
			FlipbookComponent->SetFlipbook(BodyFlipbook);
			SnakeBodyActor->SetRootComponent(FlipbookComponent);
		}
		SnakePartsArray.Add(SnakeBodyActor);
	}

	// Add to SnakePartsArray (SpawnActor result)
	// Then set IsHead
	// SetActorRotation(GetRandomInt with max of 3)
	// Select(4 options) -> SetY(Pitch)
	// 0 = 0, 1 = 90, 2 = 180, 3 = 270

}

void ASnakeGameModeBase::SpawnFood()
{
	// Get a random point within the playable area
	X_Max = (FMath::RandRange((X_Max * -1.f), X_Max) / 100.f) * 100.f;
	Z_Max = (FMath::RandRange((X_Max * -1.f), X_Max) / 100.f) * 100.f;
	//SpawnActorFromClass - class = BP_Food
	// SpawnTransformLocation (X_Max, -1, Z_Max)
	//CollisionHandling = AlwaysSpawn, IgnoreCollisions
	// Set FoodActor REF

}

void ASnakeGameModeBase::KillSnake()
{
	IsDead = true;
	for(AActor* Part : SnakePartsArray)
	{
		//get paperflipbook
		//set flipbook
		// if index 0 set to head death
		// all others set to body death
		

	}
	// Delay .5
	for (AActor* Part : SnakePartsArray)
	{
		//set all to blank flipbook
	}
}

void ASnakeGameModeBase::SelectRandomDirection()
{
	uint8 Direction = FMath::RandRange(0, 3);
	switch (Direction)
	{
	case 0:
		DirectionX = 1;
		break;
	case 1:
		DirectionX = -1;
		break;
	case 2:
		DirectionZ = 1;
		break;
	case 3:
		DirectionZ = -1;
		break;
	}
}
