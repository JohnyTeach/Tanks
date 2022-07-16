#include "TankPlayerController.h"
#include"DrawDebugHelpers.h"
#include "TankPawn.h"
#include "TanksHUD.h"

ATankPlayerController::ATankPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAxis("MoveForward",this, &ATankPlayerController::MoveForward);

	InputComponent->BindAxis("RotateRight",this, &ATankPlayerController::RotateRight);

	InputComponent->BindAction("Fire", IE_Pressed, this, &ATankPlayerController::Fire);

	InputComponent->BindAction("FireSpecial", IE_Pressed , this, &ATankPlayerController::FireSpecial);

	InputComponent->BindAction("SwapCannons", IE_Pressed, this, &ATankPlayerController::SwapCannons);

	InputComponent->BindKey(EKeys::RightMouseButton, IE_Released, this, &ATankPlayerController::OnLeftMouseButtonUp);

	InputComponent->BindAction("OpenInventory", IE_Pressed, this, &ATankPlayerController::OnInventoryClicked);
}

void ATankPlayerController::OnInventoryClicked()
{
	const auto Hud = Cast<ATanksHUD>(GetHUD());
	if (!Hud) return;

	const auto WidgetRaw = Hud->UseWidget(EWidgetID::Inventory,true, 10);
	const auto Widget = Cast<UInventoryMainWidget>(WidgetRaw);
	if (!Widget) return;

	const auto InventoryManager = TankPawn->GetInventoryManager();
	if (!InventoryManager) return;

	InventoryManager->PrepareInventoryWidget(Widget);

	//Set input mode to UI
	FInputModeUIOnly Property;
	Property.SetWidgetToFocus(Widget->TakeWidget());
	Property.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(Property);
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();	
}

void ATankPlayerController::OnLeftMouseButtonUp()
{
	if (OnMouseButtonUp.IsBound())
	{
		OnMouseButtonUp.Broadcast();
	}
}

void ATankPlayerController::MoveForward(float AxisValue)
{
	if (TankPawn)
	TankPawn->MoveForward(AxisValue);
}

void ATankPlayerController::RotateRight(float AxisValue)
{
	TankPawn->RotateRight(AxisValue);
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!GetPawn())
	{
		return;
	}
		
	FVector ScreenMousePosition;
	FVector MouseDirection;
	
	DeprojectMousePositionToWorld(ScreenMousePosition, MouseDirection);	
	auto Z = FMath::Abs(GetPawn()->GetActorLocation().Z - ScreenMousePosition.Z);	 	
	MousePosition = ScreenMousePosition - Z * MouseDirection/ MouseDirection.Z;
	
	//DrawDebugLine(GetWorld(), ScreenMousePosition, ScreenMousePosition + MouseDirection * 5000, FColor::Blue, false, 5);	
}

FVector ATankPlayerController::GetTargetLocation() const
{
	return MousePosition;
}

void ATankPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::Fire()
{
	if (TankPawn)
	{
		TankPawn->Fire();
	}
}

void ATankPlayerController::FireSpecial()
{
	if (TankPawn)
	{
		TankPawn->FireSpecial();
	}
}

void ATankPlayerController::SwapCannons()
{
	if (TankPawn)
	{
		TankPawn->SwapCannons();
		GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Red, TEXT("SWAP CANNON"));
	}
}