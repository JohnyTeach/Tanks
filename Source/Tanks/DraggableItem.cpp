#include "DraggableItem.h"
#include "PlayerStatusWidget.h"
#include "TurretDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Kismet/GameplayStatics.h"

void UDraggableItem::OnDragCancelledHandle(UDragDropOperation* Operation)
{
	SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	
	SetVisibility(ESlateVisibility::Visible);
}

void UDraggableItem::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<ATankPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ATankPlayerController* PC = Cast<ATankPlayerController>(PlayerController);
	PC->OnMouseButtonUp.AddUObject(this, &UDraggableItem::OnMouseButtonUp);
}

void UDraggableItem::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	if (ObjectNameTextBlock)
	{
		ObjectNameTextBlock->SetText(FText::FromName(ObjectName));
	}

	if (ObjectIconImage && ObjectIcon)
	{
		ObjectIconImage->Brush.SetResourceObject(ObjectIcon);
	}
}

void UDraggableItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(DraggedActor && PlayerController)
	{
		FVector WorldPosition, WorldDirection;
		PlayerController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(PlayerController->GetPawn());
		CollisionParams.AddIgnoredActor(DraggedActor);		

		GetWorld()->LineTraceSingleByChannel(HitResult, WorldPosition, WorldPosition + WorldDirection * 100'000, ECollisionChannel::ECC_WorldStatic, CollisionParams);

		if (HitResult.GetActor())
		{
			DraggedActor->SetActorLocation(HitResult.Location);
		}
		
	}
}

FReply UDraggableItem::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{		
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;		
	}
	
	if(InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
			DraggedActor = GetWorld()->SpawnActor<AActor>(DraggedActorClass);
	}
	
	return FReply::Handled();
}

void UDraggableItem::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UTurretDragDropOperation::StaticClass());
	
	if (OutOperation)
	{
		UTurretDragDropOperation* FruitDragDropOperation = Cast<UTurretDragDropOperation>(OutOperation);
		
		FruitDragDropOperation->DefaultDragVisual = this;
	
		SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.5f));
	
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
		FruitDragDropOperation->OnDragCancelled.AddDynamic( this, &UDraggableItem::OnDragCancelledHandle);		
	}
	else
	{
		Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	}
}

bool UDraggableItem::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// auto VerticalBox = Cast<UVerticalBox>(GetParent());
	// 	
	// UTurretDragDropOperation* FruitDragDropOperation = Cast<UTurretDragDropOperation>(InOperation);
	//
	// if (FruitDragDropOperation && VerticalBox && this != FruitDragDropOperation->DefaultDragVisual)
	// {
	// 	int32 IndexDragged = VerticalBox->GetChildIndex( FruitDragDropOperation->DefaultDragVisual);
	//
	// 	if (IndexDragged != -1)
	// 	{
	// 		VerticalBox->RemoveChildAt(IndexDragged);
	//
	// 		TArray<UWidget*> VBChildren = VerticalBox->GetAllChildren();
	//
	// 		VerticalBox->ClearChildren();
	// 		
	// 		for (int32 Counter = 0; Counter < VBChildren.Num(); Counter++)
	// 		{
	// 			VerticalBox->AddChildToVerticalBox(VBChildren[Counter])->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
	// 			
	// 			if (VBChildren[Counter] == this)
	// 			{
	// 				VerticalBox->AddChildToVerticalBox(FruitDragDropOperation->DefaultDragVisual)->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
	// 			}
	// 		}
	// 	}
	// }
	//
	// return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

	UVerticalBox* VerticalBox = Cast<UVerticalBox>(GetParent());
	UTurretDragDropOperation* TankOperation = Cast<UTurretDragDropOperation>(InOperation);

	if (VerticalBox && TankOperation && TankOperation->DefaultDragVisual && this != TankOperation->DefaultDragVisual)
	{
		UKismetSystemLibrary::PrintString(this, InGeometry.GetAbsolutePosition().ToString());
		const int32 OverIndex = VerticalBox->GetChildIndex(TankOperation->DefaultDragVisual);

		if (OverIndex >= 0)
		{
			VerticalBox->RemoveChildAt(OverIndex);

			const TArray<UWidget*> Children = VerticalBox->GetAllChildren();
			VerticalBox->ClearChildren();

			for (int32 i = 0; i < Children.Num(); ++i)
			{
				VerticalBox->AddChildToVerticalBox(Children[i])->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);

				if (Children[i] == this)
				{
					VerticalBox->AddChildToVerticalBox(TankOperation->DefaultDragVisual)->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
				}
			}

			return true;
		}
	}
	
	return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
}

bool UDraggableItem::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if(InOperation && InOperation->DefaultDragVisual)
	{
		UUserWidget* WidgetDrag = Cast<UUserWidget>(InOperation->DefaultDragVisual);
		
		if (WidgetDrag)
		{
			WidgetDrag->SetColorAndOpacity( FLinearColor(1.f, 1.f, 1.f, 1.f));
	
			WidgetDrag->SetVisibility(ESlateVisibility::Visible);
		}		
	}
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent,InOperation);
}

void UDraggableItem::OnMouseButtonUp()
{
	if(DraggedActor)
	{
		DraggedActor = nullptr;
	}
}