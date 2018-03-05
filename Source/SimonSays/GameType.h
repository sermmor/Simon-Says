// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SIMONSAYS_API GameType
{
public:
	//GameType();
	//virtual ~GameType() = 0;
	virtual void InitializeGame() = 0;
	virtual void UpdateGame(float DeltaTime) = 0;
	virtual void GameOver() = 0;

	virtual int GetScore() const = 0;
	virtual int GetLife() const = 0;
	virtual void TimeEnd() = 0;
	virtual bool CanGiveItem() const = 0;
	virtual void GiveItem() = 0;
	virtual bool IsInReadyState() const = 0;
	virtual bool IsInGoState() const = 0;
	virtual bool IsDeathTurn() const = 0;
	virtual bool IsPlayerTurn() const = 0;
	virtual bool IsGameOver() const = 0;
	virtual bool IsPlayerWins() const = 0;

protected:
	float CurrentTime;
	int Score, Life;
	bool IsTimeEnds, IsPlayerWinsGame, IsGameEnded, ItemReadyToGive;
};
