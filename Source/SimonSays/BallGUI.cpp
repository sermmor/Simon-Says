// Fill out your copyright notice in the Description page of Project Settings.

#include "BallGUI.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"


// Sets default values for this component's properties
UBallGUI::UBallGUI() : DynamicMatEmit(0), Sphere(0), IsInitialized(false), IsEnableClick(false), 
	IsClickBall(false), EmitCounter(0.0f), EndEmitCounter(0.01f), CurrentEmision(0.0f),
	EndEmission(50.0f), State(BALL_OFF)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UBallGUI::BeginPlay()
{
	Super::BeginPlay();

	IsInitialized = false, 
	IsEnableClick = false;
	IsClickBall = false;
	EmitCounter = 0.0f;
	EndEmitCounter = 0.01f;
	CurrentEmision = 0.0f;
	EndEmission = 50.0f;
	State = BALL_OFF;
}

void UBallGUI::InitializeBall(UMaterial* BallMaterialInterface)
{
	Super::GetOwner()->OnClicked.AddDynamic(this, &UBallGUI::OnClicInBall);

	GrabReferences(BallMaterialInterface);
	SetReferences();
	IsInitialized = true;
}

void UBallGUI::GrabReferences(UMaterial* BallMaterialInterface)
{
	Sphere = Super::GetOwner();
	CreateDynamicMaterial(BallMaterialInterface);
}

void UBallGUI::CreateDynamicMaterial(UMaterial* BallMaterialInterface)
{
	DynamicMatEmit = UMaterialInstanceDynamic::Create(BallMaterialInterface, this);
	DynamicMatEmit->SetVectorParameterValue(FName("Color"), FLinearColor(ColorRGB[0], ColorRGB[1], ColorRGB[2]));
	DynamicMatEmit->SetVectorParameterValue(FName("ColorLight"), FLinearColor(ColorLight[0], ColorLight[1], ColorLight[2]));
	DynamicMatEmit->SetScalarParameterValue(FName("Emission"), 0.0f);
}

void UBallGUI::SetReferences()
{
	TArray<UStaticMeshComponent*> meshComponent;
	Sphere->GetComponents<UStaticMeshComponent>(meshComponent);

	meshComponent[0]->SetMaterial(0, (UMaterialInterface*)DynamicMatEmit);
}

// Called every frame
void UBallGUI::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (IsInitialized)
	{
		if (State == BALL_TURNING_ON)
			UpdateEmissionOn(DeltaTime);
		else if (State == BALL_ON)
			UpdateWaitOn(DeltaTime);
		else if (State == BALL_TURNING_OFF)
			UpdateEmissionOff(DeltaTime);
	}
	
}

void UBallGUI::ResetCounters()
{
	EmitCounter = 0.0f;
	CurrentEmision = 0.0f; 
}

void UBallGUI::UpdateEmissionOn(float DeltaTime)
{
	if (CurrentEmision < EndEmission)
	{
		// Update Counter.
		EmitCounter = EmitCounter + (SpeedTurnOnAndOff * DeltaTime);
		if (EmitCounter > EndEmitCounter)
		{
			// Update Emission.
			CurrentEmision = CurrentEmision + EmissionIncrement;
			if (CurrentEmision > EndEmitCounter) CurrentEmision = EndEmission;
			DynamicMatEmit->SetScalarParameterValue(FName("Emission"), CurrentEmision);
			EmitCounter = 0.0f;
		}
	}
	else if (CurrentEmision >= EndEmission)
	{
		State = BALL_ON;
		if (IsSoundLightEnable) UGameplayStatics::PlaySound2D(GetWorld(), SoundTurnOn);
		ResetCounters();
	}
}

void UBallGUI::UpdateWaitOn(float DeltaTime)
{
	// Update Counter.
	CurrentEmision = EmitCounter + (SpeedTurnOnAndOff * DeltaTime);
	if (CurrentEmision > TimeLightOn)
	{
		// Finish time On.
		State = BALL_TURNING_OFF;
		ResetCounters();
		CurrentEmision = 50.0f;
	}
}

void UBallGUI::UpdateEmissionOff(float DeltaTime)
{
	if (CurrentEmision > 0)
	{
		// Update Counter.
		EmitCounter = EmitCounter + (SpeedTurnOnAndOff * DeltaTime);
		if (EmitCounter > EndEmitCounter)
		{
			// Update Emission.
			CurrentEmision = CurrentEmision - EmissionIncrement;
			if (CurrentEmision < 0) CurrentEmision = 0;
			DynamicMatEmit->SetScalarParameterValue(FName("Emission"), CurrentEmision);
			EmitCounter = 0.0f;
		}
	}
	else if (CurrentEmision == 0)
	{
		State = BALL_OFF;
		ResetCounters();
	}
}

UBallGUI::BallState UBallGUI::GetBallState() const
{
	return State;
}

void UBallGUI::TurnOn(bool emitSound)
{
	State = BALL_TURNING_ON;
	IsSoundLightEnable = emitSound;
}

bool UBallGUI::IsInTurnOff()
{
	return State == BALL_OFF;
}

void UBallGUI::EnableClickOnBall(bool enable)
{
	IsEnableClick = enable;
}

bool UBallGUI::IsEnableClickOnBall() const
{
	return IsEnableClick;
}

bool UBallGUI::IsClickedInBall() const
{
	return IsClickBall;
}

void UBallGUI::ResetClickedInBall()
{
	IsClickBall = false;
}

void UBallGUI::OnClicInBall(AActor* Target, FKey ButtonPressed)
{
	if (IsEnableClick && IsInTurnOff())
	{
		// When player click on ball: turn on the ball and mark as clicked.
		TurnOn(true);
		IsClickBall = true;
	}
}

void UBallGUI::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);

	// Cleaned all dynamic memory.
	if (DynamicMatEmit != NULL)
	{
		delete DynamicMatEmit;
		DynamicMatEmit = NULL;
	}
}