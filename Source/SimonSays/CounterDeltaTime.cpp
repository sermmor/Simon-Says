// Fill out your copyright notice in the Description page of Project Settings.

#include "CounterDeltaTime.h"

CounterDeltaTime::CounterDeltaTime() : CurrentTime(0), EndTime(0), IsInPause(false)
{
}

CounterDeltaTime::CounterDeltaTime(float limitTime) : CurrentTime(0), EndTime(limitTime), IsInPause(false)
{
}

bool CounterDeltaTime::Update(float DeltaTime, float velocityUpdate)
{
	if (!IsPaused() && !IsEnd())
	{
		CurrentTime += (velocityUpdate * DeltaTime);
		return true;
	}
	else
		return false;
}

bool CounterDeltaTime::IsEnd() const
{
	return CurrentTime >= EndTime;
}

bool CounterDeltaTime::IsPaused() const
{
	return IsInPause;
}

bool CounterDeltaTime::IsInTimeZero() const
{
	return CurrentTime == 0.0f;
}

bool CounterDeltaTime::IsEndWhenTime(float timeBetweenCurrentAndEnd) const
{
	return (CurrentTime + timeBetweenCurrentAndEnd) >= EndTime;
}

float CounterDeltaTime::GetEndTime() const
{
	return EndTime;
}

void CounterDeltaTime::SetEndTime(float newLimitTime)
{
	EndTime = newLimitTime;
}

void CounterDeltaTime::ForceToEnd()
{
	CurrentTime = EndTime;
}

void CounterDeltaTime::Continue()
{
	IsInPause = false;
}

void CounterDeltaTime::Pause()
{
	IsInPause = true;
}

void CounterDeltaTime::Reset()
{
	IsInPause = false;
	CurrentTime = 0.0f;
}

