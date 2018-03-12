// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallGUI.h"
#include "CounterDeltaTime.h"
#include "GameController.generated.h"

const float SECONDS_TO_LOAD = 2.0f;
const int SIZE_SECUENCE = 15;
const int GIVE_ITEM_AT_TURN = 11;
const int SCORE_POINTS_PER_BALL = 20;
const int MAX_LIFE = 3;

UCLASS()
class SIMONSAYS_API AGameController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameController();

	enum GameSelected { NONE, SIMON_SAYS };
	enum SimonSaysStates {INIT, BEGIN, WAIT_UNTIL_BEGIN_ENDS, MACHINE_TURN, 
		INIT_PLAYER_TURN, PLAYER_TURN, PLAYER_TURN_FINISHING, SHOW_OK, SHOW_FAIL, WIN_END, LOSE_END};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<UBallGUI*> AllBallsGUI;
	UPROPERTY()
	UMaterial* BallMaterialInterface;
	GameSelected GameTypeSelected;
	int SecuenceBalls[SIZE_SECUENCE];
	CounterDeltaTime GeneralCounter;
	int Score, Life;
	bool IsTimeEnds, IsPlayerWinsGame, IsGameEnded, ItemReadyToGive;
	
	void GameOver();
	void CreateRandomSecuence();

	// Simon Game.
	int SimonCurrentSecuenceSize;
	int SimonCurrentIterSecuence;
	int SimonIndexBallInTurningOn;
	int SimonIndexPlayerSecuence;
	int SimonIsPlayerTurnOk;
	SimonSaysStates SimonSaysCurrentState;
	void UpdateSimonSaysGame(float DeltaTime);

	// Strategys.
	bool StrategyWaitPostLoad(float DeltaTime);
	bool StrategyTurnOnAllLights();
	bool StrategyWaitUntilAllLightsTurnOff();
	bool StrategySimonSaysMachineTurn();
	bool StrategyClicOnBalls(bool enable);
	bool StrategyReadAndCheckPlayerSecuence();

	// Sounds.
	UPROPERTY()
	USoundBase* SoundOk;
	UPROPERTY()
	USoundBase* SoundWrong;

	UFUNCTION()
	void OnDestroyGameController(AActor* SelfActor);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Controller Params")
	TArray<AActor*> AllBalls;

	UFUNCTION(BlueprintCallable, Category = "Score")
	int GetScore() const;

	UFUNCTION(BlueprintCallable, Category = "Life")
	int GetLife() const;

	UFUNCTION(BlueprintCallable, Category = "Time")
	void TimeEnd();

	UFUNCTION(BlueprintCallable, Category = "Item")
	bool CanGiveItem() const;

	UFUNCTION(BlueprintCallable, Category = "Item")
	void GiveItem();

	UFUNCTION(BlueprintCallable, Category = "Game State")
	bool IsInReadyState() const;

	UFUNCTION(BlueprintCallable, Category = "Game State")
	bool IsInGoState() const;

	UFUNCTION(BlueprintCallable, Category = "Game State")
	bool IsDeathTurn() const;

	UFUNCTION(BlueprintCallable, Category = "Game State")
	bool IsPlayerTurn() const;

	UFUNCTION(BlueprintCallable, Category = "Game Result")
	bool IsGameOver() const;

	UFUNCTION(BlueprintCallable, Category = "Game Result")
	bool IsPlayerWins() const;
};
