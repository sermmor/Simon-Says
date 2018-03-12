// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SIMONSAYS_API CounterDeltaTime
{
public:
	CounterDeltaTime();
	/// @limitTime Time when counter ends.
	CounterDeltaTime(float limitTime);

	/// Update the counter with DeltaTime, returns if counter is continue updating.
	bool Update(float DeltaTime, float velocityUpdate);
	bool IsEnd() const;
	bool IsPaused() const;
	bool IsInTimeZero() const;
	bool IsEndWhenTime(float timeBetweenCurrentAndEnd) const;
	float GetEndTime() const;
	void SetEndTime(float newLimitTime);
	void ForceToEnd();
	void Continue();
	void Pause();
	void Reset();

private:
	float CurrentTime;
	float EndTime;
	bool IsInPause;
};
