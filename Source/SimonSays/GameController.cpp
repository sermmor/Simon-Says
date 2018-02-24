// Fill out your copyright notice in the Description page of Project Settings.

#include "GameController.h"
#include "Engine.h"


// Sets default values
AGameController::AGameController() : DynamicMatRed(0), DynamicMatBlue(0), DynamicMatGreen(0), DynamicMatYellow(0),
	DynamicMatEmitRed(0), DynamicMatEmitBlue(0), DynamicMatEmitGreen(0), DynamicMatEmitYellow(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterial> foundMaterial (TEXT("Material'/Game/Materials/BigMarble.BigMarble'"));

	if (foundMaterial.Succeeded())
	{
		BallMaterialInterface = foundMaterial.Object;
	}
}

// TODO Destructor not works, put its contains in a method for clean dinamic memory when scene changes.
/*AGameController::~AGameController()
{
	delete BallMaterialInterface;
	BallMaterialInterface = NULL;
}*/

// Called when the game starts or when spawned
void AGameController::BeginPlay()
{
	Super::BeginPlay();

	CreateDynamicMaterials();

	// TODO Search spheres and assign all Dynamnics materials.
	

}

void AGameController::CreateDynamicMaterials()
{
	DynamicMatRed = UMaterialInstanceDynamic::Create(BallMaterialInterface, this);
	DynamicMatRed->SetVectorParameterValue(FName("Color"), FLinearColor(0.25f, 0.0f, 0.0f));
	DynamicMatBlue = UMaterialInstanceDynamic::Create(BallMaterialInterface, this);
	DynamicMatBlue->SetVectorParameterValue(FName("Color"), FLinearColor(0.0f, 0.0f, 0.25f));
	DynamicMatGreen = UMaterialInstanceDynamic::Create(BallMaterialInterface, this);
	DynamicMatGreen->SetVectorParameterValue(FName("Color"), FLinearColor(0.0f, 0.25f, 0.0f));
	DynamicMatYellow = UMaterialInstanceDynamic::Create(BallMaterialInterface, this);
	DynamicMatYellow->SetVectorParameterValue(FName("Color"), FLinearColor(0.25f, 0.25f, 0.0f));

	DynamicMatEmitRed = UMaterialInstanceDynamic::Create(BallMaterialInterface, this);
	DynamicMatEmitRed->SetVectorParameterValue(FName("Color"), FLinearColor(0.25f, 0.25f, 0.0f));
	DynamicMatEmitRed->SetVectorParameterValue(FName("ColorLight"), FLinearColor(1.0f, 1.0f, 0.0f));
	DynamicMatEmitRed->SetScalarParameterValue(FName("Emission"), 50.0f);
	DynamicMatEmitBlue = UMaterialInstanceDynamic::Create(BallMaterialInterface, this);
	DynamicMatEmitBlue->SetVectorParameterValue(FName("Color"), FLinearColor(0.25f, 0.25f, 0.0f));
	DynamicMatEmitBlue->SetVectorParameterValue(FName("ColorLight"), FLinearColor(1.0f, 1.0f, 0.0f));
	DynamicMatEmitBlue->SetScalarParameterValue(FName("Emission"), 50.0f);
	DynamicMatEmitGreen = UMaterialInstanceDynamic::Create(BallMaterialInterface, this);
	DynamicMatEmitGreen->SetVectorParameterValue(FName("Color"), FLinearColor(0.25f, 0.25f, 0.0f));
	DynamicMatEmitGreen->SetVectorParameterValue(FName("ColorLight"), FLinearColor(1.0f, 1.0f, 0.0f));
	DynamicMatEmitGreen->SetScalarParameterValue(FName("Emission"), 50.0f);
	DynamicMatEmitYellow = UMaterialInstanceDynamic::Create(BallMaterialInterface, this);
	DynamicMatEmitYellow->SetVectorParameterValue(FName("Color"), FLinearColor(0.25f, 0.25f, 0.0f));
	DynamicMatEmitYellow->SetVectorParameterValue(FName("ColorLight"), FLinearColor(1.0f, 1.0f, 0.0f));
	DynamicMatEmitYellow->SetScalarParameterValue(FName("Emission"), 50.0f);
}

// Called every frame
void AGameController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


