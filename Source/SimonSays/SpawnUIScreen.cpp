// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnUIScreen.h"
#include "Blueprint/UserWidget.h"
#include "TextWidgetTypes.h"
#include "TextBlock.h"

// Sets default values
ASpawnUIScreen::ASpawnUIScreen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnUIScreen::BeginPlay()
{
	Super::BeginPlay();
	
	// Setting widgets.
	if (WidgetObjToSpawn)
	{
		pToSpawnWidget = CreateWidget<UUserWidget>(GetGameInstance(), WidgetObjToSpawn);

		if (pToSpawnWidget.IsValid())
		{
			pToSpawnWidget->AddToViewport();
			if (IsGameScreen)
			{
				pScoreText = (UTextBlock*) pToSpawnWidget->GetWidgetFromName("TextScore");
			}
		}
	}
}

// Called every frame
void ASpawnUIScreen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnUIScreen::SetScoreText(int Score)
{
	if (pScoreText.IsValid())
		pScoreText->SetText(FText::FromString(FString::FromInt(Score)));
}

TWeakObjectPtr<class UUserWidget> ASpawnUIScreen::GetWidget() const
{
	return pToSpawnWidget;
}

