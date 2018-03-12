// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameType.h"
#include "CoreMinimal.h"

/**
 * 
 */
class SIMONSAYS_API SimonGameType : public GameType
{
public:
	SimonGameType();
	virtual ~SimonGameType();

	void InitializeGame() override;
	void UpdateGame(float DeltaTime) override;
	void GameOver() override;

	int GetScore() const override;
	int GetLife() const override;
	void TimeEnd() override;
	bool CanGiveItem() const override;
	void GiveItem() override;
	bool IsInReadyState() const override;
	bool IsInGoState() const override;
	bool IsDeathTurn() const override;
	bool IsPlayerTurn() const override;
	bool IsGameOver() const override;
	bool IsPlayerWins() const override;
};
