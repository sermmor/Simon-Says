// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallGUI.h"
#include "GameController.generated.h"

const int SIZE_SECUENCE = 20;
const int SCORE_POINTS_PER_BALL = 20;
const int MAX_LIFE = 3;

UCLASS()
class SIMONSAYS_API AGameController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameController();

	enum GameType { NONE, SIMON_SAYS };
	enum SimonSaysStates {BEGIN, WAIT_UNTIL_BEGIN_ENDS, MACHINE_TURN, 
		INIT_PLAYER_TURN, PLAYER_TURN, PLAYER_TURN_FINISHING, SHOW_OK, SHOW_FAIL, WIN_END, LOSE_END};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<UBallGUI*> AllBallsGUI;
	UMaterial* BallMaterialInterface;
	GameType GameTypeSelected;
	int SecuenceBalls[SIZE_SECUENCE];
	int Score, Life;

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
	bool StrategyTurnOnAllLights();
	bool StrategyWaitUntilAllLightsTurnOff();
	bool StrategySimonSaysMachineTurn();
	bool StrategyClicOnBalls(bool enable);
	bool StrategyReadAndCheckPlayerSecuence();

	// Sounds.
	USoundBase* SoundOk;
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
};
