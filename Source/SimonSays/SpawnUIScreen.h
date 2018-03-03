// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnUIScreen.generated.h"

UCLASS()
class SIMONSAYS_API ASpawnUIScreen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnUIScreen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> WidgetObjToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type Screen")
	bool IsGameScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time options")
	int TimeInit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time options")
	float SpeedTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time options")
	float TimeToDecrementTimer;

	TWeakObjectPtr<class UUserWidget> GetWidget() const;

private:
	// Variable to hold the widget after creating it
	TWeakObjectPtr<class UUserWidget> pToSpawnWidget;
	TWeakObjectPtr<class UTextBlock> pTimeText;
	TWeakObjectPtr<class UTextBlock> pScoreText;

	int CurrentTime;
	float CounterToDecrementTimer;
	bool PlayerIsReady;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void SetScoreText(int score);

	UFUNCTION(BlueprintCallable, Category = "Time")
	bool IsTimeEnds();

	UFUNCTION(BlueprintCallable, Category = "Item")
	void UseItemTime();

	UFUNCTION(BlueprintCallable, Category = "State")
	void SetPlayerReady();

	void UpdateTimeCounter(float DeltaTime);
	void SetTimeText(int newTime);
};
