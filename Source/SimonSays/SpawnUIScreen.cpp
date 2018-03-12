// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnUIScreen.h"
#include "Blueprint/UserWidget.h"
#include "TextWidgetTypes.h"
#include "TextBlock.h"
#include "Image.h"

// Sets default values
ASpawnUIScreen::ASpawnUIScreen() : CurrentTime(0), CounterToDecrementTimer(0.0f), PlayerIsReady(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnUIScreen::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerIsReady = false;
	CurrentTime = TimeInit;
	CounterToDecrementTimer = 0.0f;

	// Setting widgets.
	if (WidgetObjToSpawn)
	{
		pToSpawnWidget = CreateWidget<UUserWidget>(GetGameInstance(), WidgetObjToSpawn);

		if (pToSpawnWidget.IsValid())
		{
			pToSpawnWidget->AddToViewport();
			if (IsGameScreen)
			{
				//UImage img;
				pScoreText = (UTextBlock*) pToSpawnWidget->GetWidgetFromName("TextScore");
				pTimeText = (UTextBlock*) pToSpawnWidget->GetWidgetFromName("TextTime");
			}
		}
	}

	SetTimeText(CurrentTime);
}

// Called every frame
void ASpawnUIScreen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsGameScreen && PlayerIsReady)
		UpdateTimeCounter(DeltaTime);
}

void ASpawnUIScreen::UpdateTimeCounter(float DeltaTime)
{
	if (CurrentTime > 0)
	{
		CounterToDecrementTimer += (DeltaTime * SpeedTimer);
		if (CounterToDecrementTimer >= TimeToDecrementTimer)
		{
			CurrentTime--;
			CounterToDecrementTimer = 0.0f;
			SetTimeText(CurrentTime);
		}
	}
}

void ASpawnUIScreen::SetTimeText(int newTime)
{
	if (pTimeText.IsValid())
		pTimeText->SetText(FText::FromString(FString::FromInt(newTime)));
}

void ASpawnUIScreen::SetScoreText(int Score)
{
	if (pScoreText.IsValid())
		pScoreText->SetText(FText::FromString(FString::FromInt(Score)));
}

void ASpawnUIScreen::SetPlayerReady()
{
	PlayerIsReady = true;
}

void ASpawnUIScreen::UseItemTime()
{
	CurrentTime += 50;
}

TWeakObjectPtr<class UUserWidget> ASpawnUIScreen::GetWidget() const
{
	return pToSpawnWidget;
}

bool ASpawnUIScreen::IsTimeEnds()
{
	return CurrentTime <= 0;
}