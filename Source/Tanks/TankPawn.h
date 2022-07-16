#pragma once
#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "KillableActor.h"
#include "QuestDialog.h"
#include "QuestList.h"
#include "UniversalPawn.h"
#include "TankPawn.generated.h"

UCLASS()
class TANKS_API ATankPawn : public AUniversalPawn, public IInteractableObject, public IKillableActor
{
	GENERATED_BODY()

public:

	ATankPawn();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	USpringArmComponent * SpringArm;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UCameraComponent * Camera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Cannon")
	UAudioComponent* SwapCannonAudioEffect;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UEquipInventoryComponent* InventoryEquipComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UInventoryManagerComponent* InventoryManagerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Movement", meta = (EditCondition = "bIsTankPawn", EditConditionHides))
	float MovementSpeed = 250;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Movement", meta = (EditCondition = "bIsTankPawn", EditConditionHides))
	float RotationSpeed = 250;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Movement", meta = (EditCondition = "bIsTankPawn", EditConditionHides))
	float MovementAcceleration = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Movement", meta = (EditCondition = "bIsTankPawn", EditConditionHides))
	float RotationAcceleration = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float TurretRotationAcceleration = 0.05f; 	
		
	float ForwardScaleMax = 0;
	float ForwardScaleCurrent = 0;
	float RotateScaleMax = 0;
	float RotateScaleCurrent = 0;

	UPROPERTY()
	ACannon* SecondaryCannon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	FName WaypointTag;	

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category ="Components")
	UQuestListComponent* QuestListComp;

	UFUNCTION(BlueprintCallable)
	void ToggleQuestListVisibility();
	
	UPROPERTY()
	UQuestList* QuestList;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UQuestList> QuestListClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scorable")
	int32 DestructionScores = 10;

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	virtual void TakeDamage(const FDamageInfo& DamageInfo) override;

	UFUNCTION(BlueprintCallable, Category = "Cannon")
	void SwapCannons();

	UFUNCTION()
	void MoveForward(float AxisValue);
	
	UFUNCTION()
	void RotateRight(float AxisValue);

	UInventoryManagerComponent* GetInventoryManager();

	UStaticMeshComponent* GetEquipComponent(EItemEquipSlot Slot);
	
	virtual void EquipItem(EItemEquipSlot Slot, FName ItemID) override;
	
	virtual void UnEquipItem(EItemEquipSlot Slot, FName ItemID) override;

	virtual int32 GetScores() override;
};

