// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BallGUI.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMONSAYS_API UBallGUI : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBallGUI();

	enum BallState { BALL_TURNING_ON, BALL_ON, BALL_TURNING_OFF, BALL_OFF};

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball GUI Params")
	TArray<float> ColorRGB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball GUI Params")
	TArray<float> ColorLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball GUI Params")
	float SpeedTurnOnAndOff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball GUI Params")
	float EmissionIncrement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball GUI Params")
	float TimeLightOn; // Time with the light on.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball GUI Params")
	USoundBase* SoundTurnOn; // Sound when ball turn on.

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Executed when component destroyed.
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	// Initialize the ball with a template material.
	void InitializeBall(UMaterial* BallMaterialInterface);

	BallState GetBallState() const;
	void TurnOn(bool EmitSound);
	bool IsInTurnOff();

	void EnableClickOnBall(bool enable);
	bool IsEnableClickOnBall() const;
	bool IsClickedInBall() const;
	void ResetClickedInBall();
private:
	UMaterialInstanceDynamic* DynamicMatEmit;
	TWeakObjectPtr<AActor> Sphere;
	BallState State;

	bool IsInitialized;
	bool IsEnableClick;
	bool IsClickBall;
	bool IsSoundLightEnable;

	float EmitCounter;
	float EndEmitCounter;
	float CurrentEmision;
	float EndEmission;

	void GrabReferences(UMaterial* BallMaterialInterface);
	void CreateDynamicMaterial(UMaterial* BallMaterialInterface);
	void SetReferences();
	void ResetCounters();
	void UpdateEmissionOn(float DeltaTime);
	void UpdateWaitOn(float DeltaTime);
	void UpdateEmissionOff(float DeltaTime);

	UFUNCTION()
	void OnClicInBall(AActor* Target, FKey ButtonPressed);
};
