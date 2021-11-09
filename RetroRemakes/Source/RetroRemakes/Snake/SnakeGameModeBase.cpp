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
	SelectRandomDirection();
	SpawnSnakeHead();
	SpawnFood();

}

void ASnakeGameModeBase::SpawnSnakeHead()
{
	SnakePartsArray.Empty();
	FVector StartingLocation = FVector(0, 1, 0);
	SpawnNewSnakePart(StartingLocation, true);
}

// Add a snake body part at the given location
void ASnakeGameModeBase::SpawnNewSnakePart(FVector SpawnLocation, bool IsHead)
{
	// Spawn a SnakeBody actor at the given location while ignoring collision
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* SnakeBodyActor = GetWorld()->SpawnActor(SnakeBodyActorClass, &SpawnLocation, &FRotator::ZeroRotator, SpawnParams);

	if (SnakeBodyActor != nullptr)
	{
		if (!IsHead && IsValid(BodyFlipbook))
		{
			FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook Component"));
			FlipbookComponent->SetFlipbook(BodyFlipbook);
			SnakeBodyActor->SetRootComponent(FlipbookComponent);
		}
		if (IsHead && IsValid(HeadFlipbook))
		{
			FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook Component"));
			FlipbookComponent->SetFlipbook(HeadFlipbook);
			SnakeBodyActor->SetRootComponent(FlipbookComponent);
			IsHead = false;
		}
		
		SnakePartsArray.Add(SnakeBodyActor);
	}

	// If using a simple sprite, can use the below to rotate it
	// SetActorRotation(GetRandomInt with max of 3)
	// Select(4 options) -> SetY(Pitch)
	// 0 = 0, 1 = 90, 2 = 180, 3 = 270
}

// Get a random location within the play area and spawn a FoodActor
void ASnakeGameModeBase::SpawnFood()
{
	// Get a random point within the playable area that is a multiple of 20 and add 10 to line it upo in the grid squares
	X_Max = ((FMath::RandRange((X_Max * -1), X_Max) / 20) * 20) + 10;
	Z_Max = ((FMath::RandRange((X_Max * -1), X_Max) / 20) * 20) + 10;

	// Spawn a Food actor at the given location while ignoring collision
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector SpawnLocation = FVector(X_Max, 1, Z_Max);
	AActor* FoodActor = GetWorld()->SpawnActor(FoodActorClass, &SpawnLocation, &FRotator::ZeroRotator, SpawnParams);
}

// Get the component from the actor, which should be a UPaperFlipbookComponent and update the flipbook
void ASnakeGameModeBase::KillSnake()
{
	IsDead = true;
	for(int i = 0; i < SnakePartsArray.Num() - 1; i++)
	{
		AActor* PartActor = SnakePartsArray[i];
		UPaperFlipbookComponent* ToKillComponent = Cast<UPaperFlipbookComponent>(PartActor->GetRootComponent());
		if (i == 0)
		{
			ToKillComponent->SetFlipbook(HeadDeathFlipbook);
		} 
		else
		{
			ToKillComponent->SetFlipbook(BodyDeathFlipbook);
		}
	}

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&] {SetBlankFlipbook(); });
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.5f, false);
}

// Helper method for KillSnake. Updates the flipbook to a blank flipbook.
void ASnakeGameModeBase::SetBlankFlipbook()
{
	for (AActor* Part : SnakePartsArray)
	{
		UPaperFlipbookComponent* ToSetBlankComponent = Cast<UPaperFlipbookComponent>(Part->GetRootComponent());
		ToSetBlankComponent->SetFlipbook(BlankFlipbook);
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
