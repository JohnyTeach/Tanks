#pragma once

#include "CoreMinimal.h"
#include "TargetController.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class ATankPawn;

UCLASS()
class TANKS_API ATankPlayerController : public APlayerController, public ITargetController
{
	GENERATED_BODY()
	
public:

	FSimpleMulticastDelegate OnMouseButtonUp;

	void OnLeftMouseButtonUp();
	
	ATankPlayerController();
	
	virtual void BeginPlay() override;
	
	virtual  void SetupInputComponent() override;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual FVector GetTargetLocation() const override;

	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY()
	ATankPawn* TankPawn;
	
	UPROPERTY()
	FVector MousePosition;	
	
	void MoveForward(float AxisValue);
	
	void RotateRight(float AxisValue);
	
	void Fire();
	
	void FireSpecial();
	
	void SwapCannons();

	UFUNCTION()
	void OnInventoryClicked();
};