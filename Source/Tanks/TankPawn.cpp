#include "TankPawn.h"
#include "QuestListComponent.h"
#include "Misc/TextFilterExpressionEvaluator.h"
#include "Quest.h"
#include "TargetController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ATankPawn::ATankPawn()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	SwapCannonAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Swap Cannon Audio Effect"));

	EngineAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Engine Audio Effect"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
	
	InventoryEquipComponent = CreateDefaultSubobject<UEquipInventoryComponent>(TEXT("Inventory Equip Component"));
	
	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>(TEXT("Inventory Manager Component"));	
	
	QuestListComp = CreateDefaultSubobject<UQuestListComponent>(TEXT("Quest List Component"));
}

void ATankPawn::ToggleQuestListVisibility()
{
	APlayerController * PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
    if (QuestList)
    {
	    QuestList->RemoveFromParent();
    	
	    QuestList = nullptr;
    	
	   //  UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC);
       // 
	   // PC->bShowMouseCursor = false;
    }
    else
    {
    	if (QuestListClass)
    	{
    		QuestList = CreateWidget<UQuestList>(GetWorld(), QuestListClass);
    		
    		QuestList->Init(QuestListComp);
    		
    		QuestList->AddToViewport();
    		
    		// UWidgetBlueprintLibrary::SetInputMode_GameAndUI(PC);
    		//
    		// PC->bShowMouseCursor = true;
    	}
    }
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	SetupCannon(CannonClass);

	EngineAudioEffect->Activate();

	if (GetWorld()->GetFirstPlayerController() == ATankPawn::GetController())
	{
		if (InventoryComponent && InventoryEquipComponent && InventoryManagerComponent)
		{
			InventoryComponent->Init();
			InventoryManagerComponent->SetBaseInventory(InventoryComponent);

			InventoryEquipComponent->Init();
			InventoryManagerComponent->SetBaseEquipment(InventoryEquipComponent);
		}				
	}	
}

void ATankPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// движение вперед/назад	
	auto Location = GetActorLocation(); 
	auto ForwardVector = GetActorForwardVector();
	ForwardScaleCurrent = FMath::Lerp(ForwardScaleCurrent, ForwardScaleMax, MovementAcceleration); 
	SetActorLocation(Location + ForwardVector * ForwardScaleCurrent * MovementSpeed * DeltaSeconds, true);	
	
	// развороты
	auto Rotation = GetActorRotation(); 
	RotateScaleCurrent = FMath::Lerp(RotateScaleCurrent, RotateScaleMax, RotationAcceleration);
	Rotation.Yaw = Rotation.Yaw + RotationSpeed * RotateScaleCurrent * DeltaSeconds; SetActorRotation(Rotation); 

	// поворот башни
	if(TargetController)
	{
		auto MousePosition = TargetController->GetTargetLocation(); 
		auto TurretRotation = Turret->GetComponentRotation(); 
		FRotator MouseRotation = UKismetMathLibrary::FindLookAtRotation(Turret->GetComponentLocation(), MousePosition);
		MouseRotation.Roll = TurretRotation.Roll; 
		MouseRotation.Pitch = TurretRotation.Pitch;

		Turret->SetWorldRotation(FMath::Lerp(TurretRotation, MouseRotation, TurretRotationAcceleration));			
	}
}

void ATankPawn::TakeDamage(const FDamageInfo& DamageInfo)
{
	Super::TakeDamage(DamageInfo);

	if (HealthComponent && HealthComponent->GetHealth() <= 0)
	{
		KilledActor(this, DamageInfo.Instigator);
	}
}

void ATankPawn::SwapCannons()
{	
	Swap(Cannon, SecondaryCannon);
	
	SwapCannonAudioEffect->Activate(true);

	if (Cannon)
	{
		Cannon->SetVisibility(true);
	}

	if (SecondaryCannon)
	{
		SecondaryCannon->SetVisibility(false);
	}			
}

void ATankPawn::MoveForward(float AxisValue)
{
	ForwardScaleMax = AxisValue;	
}

void ATankPawn::RotateRight(float AxisValue)
{
	RotateScaleMax = AxisValue;	
}

UInventoryManagerComponent* ATankPawn::GetInventoryManager()
{
	if (GetWorld()->GetFirstPlayerController() == ATankPawn::GetController())
	{
		return InventoryManagerComponent;
	}

	return nullptr;
}

void ATankPawn::EquipItem(EItemEquipSlot Slot, FName ItemID)
{	
	const auto Component = GetEquipComponent(Slot);
	if (!Component) return;

	const auto ItemInfo = InventoryManagerComponent->GetItemData(ItemID);
	
	Component->SetStaticMesh(ItemInfo->Mesh.LoadSynchronous());
	
	Component->SetHiddenInGame(false);	
}

void ATankPawn::UnEquipItem(EItemEquipSlot Slot, FName ItemID)
{	
	const auto Component = GetEquipComponent(Slot);
	if (!Component) return;

	Component->SetStaticMesh(nullptr);
	
	Component->SetHiddenInGame(false);
}

UStaticMeshComponent* ATankPawn::GetEquipComponent(EItemEquipSlot Slot)
{
	FName EquipTag = "";

	switch (Slot)
	{
	case EItemEquipSlot::Weapon: EquipTag = "Equip_Weapon";
		break;
	case EItemEquipSlot::Wheels: EquipTag = "Equip_Wheels";
		break;
	case EItemEquipSlot::Armor: EquipTag = "Equip_Armor";
		break;
	case EItemEquipSlot::None: break;
		
	default: return nullptr;
	}

	auto Components = GetComponentsByTag(UStaticMeshComponent::StaticClass(), EquipTag);
	
	return Components.Num() > 0 ? Cast<UStaticMeshComponent>(Components[0]) : nullptr;
}

int32 ATankPawn::GetScores()
{
	return DestructionScores;
}

