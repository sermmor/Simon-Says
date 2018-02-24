// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameController.generated.h"

UCLASS()
class SIMONSAYS_API AGameController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameController();

	// TODO Destructor not works, put its contains in a method for clean dinamic memory when scene changes.
	//~AGameController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UMaterial* BallMaterialInterface;

	TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMatRed;
	TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMatBlue;
	TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMatGreen;
	TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMatYellow;
	TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMatEmitRed;
	TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMatEmitBlue;
	TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMatEmitGreen;
	TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMatEmitYellow;

	void CreateDynamicMaterials();

};
