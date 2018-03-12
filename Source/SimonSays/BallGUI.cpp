// Fill out your copyright notice in the Description page of Project Settings.

#include "BallGUI.h"
#include "Engine.h"


// Sets default values for this component's properties
UBallGUI::UBallGUI() : DynamicMatEmitOff(0), DynamicMatEmitOn(0), DynamicMatEmitFromOffToOn(0),
	meshComponent(0), Sphere(0), IsInitialized(false), IsEnableClick(false),
	IsClickBall(false), EmitCounterDT(0.01f), State(BALL_OFF)
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
	EmitCounterDT.SetEndTime(TimeLightTurningOnOff);
	State = BALL_OFF;
}

void UBallGUI::InitializeGamepad()
{
	if (InputComp)
	{
		//Do whatever with myInputComp if it's valid.
		FName NameActionBt = FName(*NameActionButton);
		InputComp->BindAction(NameActionBt, EInputEvent::IE_Released, this, &UBallGUI::OnPushInBall);//"ActionA"
	}
}

void UBallGUI::InitializeBall(UMaterial* BallMaterialInterface, UInputComponent* ic)
{
	InputComp = ic;
	Super::GetOwner()->OnClicked.AddDynamic(this, &UBallGUI::OnClicInBall);
	InitializeGamepad();

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
	DynamicMatEmitOff = UMaterialInstanceDynamic::Create(BallMaterialInterface, this);
	DynamicMatEmitOff->SetVectorParameterValue(FName("Color"), FLinearColor(ColorRGB[0], ColorRGB[1], ColorRGB[2]));
	DynamicMatEmitOff->SetVectorParameterValue(FName("ColorLight"), FLinearColor(ColorLight[0], ColorLight[1], ColorLight[2]));
	DynamicMatEmitOff->SetScalarParameterValue(FName("Emission"), 0.0f);

	DynamicMatEmitFromOffToOn = UMaterialInstanceDynamic::Create(BallMaterialInterface, this);
	DynamicMatEmitFromOffToOn->SetVectorParameterValue(FName("Color"), FLinearColor(ColorRGB[0], ColorRGB[1], ColorRGB[2]));
	DynamicMatEmitFromOffToOn->SetVectorParameterValue(FName("ColorLight"), FLinearColor(ColorLight[0], ColorLight[1], ColorLight[2]));
	DynamicMatEmitFromOffToOn->SetScalarParameterValue(FName("Emission"), 10.0f);

	DynamicMatEmitOn = UMaterialInstanceDynamic::Create(BallMaterialInterface, this);
	DynamicMatEmitOn->SetVectorParameterValue(FName("Color"), FLinearColor(ColorRGB[0], ColorRGB[1], ColorRGB[2]));
	DynamicMatEmitOn->SetVectorParameterValue(FName("ColorLight"), FLinearColor(ColorLight[0], ColorLight[1], ColorLight[2]));
	DynamicMatEmitOn->SetScalarParameterValue(FName("Emission"), 50.0f);
}

void UBallGUI::SetReferences()
{
	TArray<UStaticMeshComponent*> meshComponentArray;
	Sphere->GetComponents<UStaticMeshComponent>(meshComponentArray);

	meshComponent = meshComponentArray[0];

	meshComponent->SetMaterial(0, (UMaterialInterface*)DynamicMatEmitOff);
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

void UBallGUI::UpdateEmissionOn(float DeltaTime)
{
	if (EmitCounterDT.IsInTimeZero())
		meshComponent->SetMaterial(0, (UMaterialInterface*)DynamicMatEmitFromOffToOn);

	EmitCounterDT.Update(DeltaTime, SpeedTurnOnAndOff);

	if (EmitCounterDT.IsEnd())
	{
		// Go to turn on state.
		meshComponent->SetMaterial(0, (UMaterialInterface*)DynamicMatEmitOn);
		State = BALL_ON;
		if (IsSoundLightEnable) UGameplayStatics::PlaySound2D(GetWorld(), SoundTurnOn);
		EmitCounterDT.SetEndTime(TimeLightOn);
		EmitCounterDT.Reset();
	}
}

void UBallGUI::UpdateWaitOn(float DeltaTime)
{
	// Update Counter.
	EmitCounterDT.Update(DeltaTime, SpeedTurnOnAndOff);
	if (EmitCounterDT.IsEnd())
	{
		// Finish time On => Go to turning off state.
		State = BALL_TURNING_OFF;
		EmitCounterDT.SetEndTime(TimeLightTurningOnOff);
		EmitCounterDT.Reset();
	}
}

void UBallGUI::UpdateEmissionOff(float DeltaTime)
{
	if (EmitCounterDT.IsInTimeZero())
		meshComponent->SetMaterial(0, (UMaterialInterface*)DynamicMatEmitFromOffToOn);

	EmitCounterDT.Update(DeltaTime, SpeedTurnOnAndOff);

	if (EmitCounterDT.IsEnd())
	{
		// Go to turn off state.
		meshComponent->SetMaterial(0, (UMaterialInterface*)DynamicMatEmitOff);
		State = BALL_OFF;
		EmitCounterDT.Reset();
	}
}

UBallGUI::BallState UBallGUI::GetBallState() const
{
	return State;
}

void UBallGUI::TurnOn(bool emitSound)
{
	// Enable turning on state.
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
	OnPushInBall();
}

void UBallGUI::OnPushInBall()
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
	if (DynamicMatEmitOff != NULL)
	{
		delete DynamicMatEmitOff;
		DynamicMatEmitOff = NULL;
	}

	if (meshComponent != NULL)
		meshComponent = NULL;

	if (DynamicMatEmitFromOffToOn != NULL)
		DynamicMatEmitFromOffToOn = NULL;
	
	if (DynamicMatEmitOn != NULL)
		DynamicMatEmitOn = NULL;

	if (InputComp != NULL)
		InputComp = NULL;
}