// Fill out your copyright notice in the Description page of Project Settings.

#include "GameController.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGameController::AGameController() : SimonSaysCurrentState(BEGIN)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterial> foundMaterial (TEXT("Material'/Game/Materials/BigMarble.BigMarble'"));
	if (foundMaterial.Succeeded())
		BallMaterialInterface = foundMaterial.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> foundSoundOk(TEXT("SoundWave'/Game/Puzzle/Sounds/SoundOK.SoundOK'"));
	if (foundSoundOk.Succeeded())
		SoundOk = foundSoundOk.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> foundSoundWrong(TEXT("SoundWave'/Game/Puzzle/Sounds/SoundWrong.SoundWrong'"));
	if (foundSoundWrong.Succeeded())
		SoundWrong = foundSoundWrong.Object;

	OnDestroyed.AddDynamic(this, &AGameController::OnDestroyGameController);
}

void AGameController::CreateRandomSecuence()
{
	int lastRandom = -1, newRandom, counterLastRandomEquals = 0;
	for (int i = 0; i < SIZE_SECUENCE; i++)
	{
		newRandom = FMath::RandRange(0,3);
		if (lastRandom != newRandom || counterLastRandomEquals < 3)
		{
			// Not repeat the same random ball more than 3 times.
			SecuenceBalls[i] = newRandom;
			if (lastRandom == newRandom)
				counterLastRandomEquals++;
			else
				counterLastRandomEquals = 0;

			lastRandom = newRandom;
		}
		else
			i--; // If the same ball repeats generate other random for the same index.
	}

	/*//Secuence only for testing:
	SecuenceBalls[0] = 0; SecuenceBalls[1] = 1; SecuenceBalls[2] = 2; SecuenceBalls[3] = 3; SecuenceBalls[4] = 3;
	SecuenceBalls[5] = 0; SecuenceBalls[6] = 3; SecuenceBalls[7] = 2; SecuenceBalls[8] = 1; SecuenceBalls[9] = 0;
	SecuenceBalls[10] = 0; SecuenceBalls[11] = 0; SecuenceBalls[12] = 2; SecuenceBalls[13] = 3; SecuenceBalls[14] = 2;
	SecuenceBalls[15] = 2; SecuenceBalls[16] = 2; SecuenceBalls[17] = 2; SecuenceBalls[18] = 1; SecuenceBalls[19] = 0;*/
}

// Called when the game starts or when spawned
void AGameController::BeginPlay()
{
	Super::BeginPlay();

	CreateRandomSecuence();

	TArray<UBallGUI*> BallComponents;
	for (AActor* ball : AllBalls)
	{
		ball->GetComponents<UBallGUI>(BallComponents);
		BallComponents[0]->InitializeBall(BallMaterialInterface);
		AllBallsGUI.Add(BallComponents[0]);
	}
	
	GameTypeSelected = SIMON_SAYS; // TODO Minimun value product. For final version THIS should be selected by the player.
}

// Called every frame
void AGameController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameTypeSelected == SIMON_SAYS)
		UpdateSimonSaysGame(DeltaTime);
}

void AGameController::UpdateSimonSaysGame(float DeltaTime)
{
	if (SimonCurrentSecuenceSize >= SIZE_SECUENCE)
		SimonSaysCurrentState = WIN_END; // TODO Change to records screen when game finished.

	if (SimonSaysCurrentState == BEGIN && StrategyTurnOnAllLights())
	{
		SimonCurrentSecuenceSize = 1;
		SimonCurrentIterSecuence = 0;
		SimonIndexBallInTurningOn = SecuenceBalls[SimonCurrentIterSecuence];
		SimonSaysCurrentState = WAIT_UNTIL_BEGIN_ENDS;
		UGameplayStatics::PlaySound2D(GetWorld(), SoundOk);
	}
	else if (SimonSaysCurrentState == WAIT_UNTIL_BEGIN_ENDS && StrategyWaitUntilAllLightsTurnOff())
	{
		SimonSaysCurrentState = MACHINE_TURN;
	}
	else if (SimonSaysCurrentState == MACHINE_TURN && StrategySimonSaysMachineTurn())
	{
		SimonSaysCurrentState = INIT_PLAYER_TURN;
	}
	else if (SimonSaysCurrentState == INIT_PLAYER_TURN && StrategyWaitUntilAllLightsTurnOff() && StrategyClicOnBalls(true))
	{
		SimonIndexPlayerSecuence = 0;
		SimonSaysCurrentState = PLAYER_TURN;
	}
	else if (SimonSaysCurrentState == PLAYER_TURN && StrategyReadAndCheckPlayerSecuence())
	{
		SimonSaysCurrentState = PLAYER_TURN_FINISHING;
	}
	else if (SimonSaysCurrentState == PLAYER_TURN_FINISHING && StrategyWaitUntilAllLightsTurnOff() && StrategyTurnOnAllLights())
	{
		// Secuence finished, prepared for the next secuence.
		SimonCurrentIterSecuence = 0;
		SimonCurrentSecuenceSize++;
		SimonSaysCurrentState = WAIT_UNTIL_BEGIN_ENDS;

		if (SimonIsPlayerTurnOk)
		{
			// TODO Increase score.
			UGameplayStatics::PlaySound2D(GetWorld(), SoundOk);
		}
		else
		{
			// TODO Decrement a live.
			UGameplayStatics::PlaySound2D(GetWorld(), SoundWrong);
			// Reset game.
			SimonCurrentSecuenceSize = 1;
			SimonCurrentIterSecuence = 0;
			SimonIndexBallInTurningOn = SecuenceBalls[SimonCurrentIterSecuence];
		}
	}
}

bool AGameController::StrategyTurnOnAllLights()
{
	for (UBallGUI* ball : AllBallsGUI)
	{
		ball->TurnOn();
	}
	return true;
}

bool AGameController::StrategyWaitUntilAllLightsTurnOff()
{
	bool isAllLightTurnOff = true;
	for (UBallGUI* ball : AllBallsGUI)
	{
		if (!ball->IsInTurnOff())
		{
			isAllLightTurnOff = false;
			break;
		}
	}

	return isAllLightTurnOff;
}

bool AGameController::StrategySimonSaysMachineTurn()
{
	if (SimonCurrentIterSecuence < SimonCurrentSecuenceSize)
	{
		if (AllBallsGUI[SimonIndexBallInTurningOn]->IsInTurnOff())
		{
			// Turn on other light.
			SimonIndexBallInTurningOn = SecuenceBalls[SimonCurrentIterSecuence];
			AllBallsGUI[SimonIndexBallInTurningOn]->TurnOn();
			SimonCurrentIterSecuence++;
		}
		return false;
	}
	else
	{
		return true;
	}

}

bool AGameController::StrategyClicOnBalls(bool enable)
{
	for (UBallGUI* ball : AllBallsGUI)
	{
		ball->EnableClickOnBall(enable);
		if (!enable)
			ball->ResetClickedInBall();
	}
	return true;
}

bool AGameController::StrategyReadAndCheckPlayerSecuence()
{
	// Search a ball
	int ballIndexClicked = -1;
	for (int i = 0; i < AllBallsGUI.Num(); i++)
	{
		if (AllBallsGUI[i]->IsClickedInBall())
		{
			ballIndexClicked = i;
		}
		AllBallsGUI[i]->ResetClickedInBall();
	}

	if (ballIndexClicked == -1)
		return false;
	else
	{
		SimonIsPlayerTurnOk = (ballIndexClicked == SecuenceBalls[SimonIndexPlayerSecuence]);
		bool isPlayerFinished = SimonIndexPlayerSecuence >= (SimonCurrentSecuenceSize - 1);
		if (!isPlayerFinished) 
			SimonIndexPlayerSecuence++;
		return isPlayerFinished || !SimonIsPlayerTurnOk;
	}
}

void AGameController::OnDestroyGameController(AActor* SelfActor)
{
	// Cleaned all dynamic memory.
	if (BallMaterialInterface != NULL)
	{
		delete BallMaterialInterface;
		BallMaterialInterface = NULL;
	}

	if (SoundOk != NULL)
	{
		delete SoundOk;
		SoundOk = NULL;
	}

	if (SoundWrong != NULL)
	{
		delete SoundWrong;
		SoundWrong = NULL;
	}

	for (UBallGUI* ball : AllBallsGUI)
	{
		ball = NULL;
	}
}

