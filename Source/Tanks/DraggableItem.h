#pragma once

#include "CoreMinimal.h"
#include "TankPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DraggableItem.generated.h"

UCLASS()
class TANKS_API UDraggableItem : public UUserWidget
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DraggedActorClass;

	UPROPERTY()
	AActor* DraggedActor;

	UPROPERTY()
	ATankPlayerController* PlayerController;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* ObjectNameTextBlock;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* ObjectIconImage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Settings")
	FName ObjectName;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Settings")
	FText ObjectDescription;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Settings")
	UTexture2D* ObjectIcon;

	UFUNCTION()
	void OnDragCancelledHandle(UDragDropOperation* Operation);

	virtual void NativeConstruct() override;

	virtual void NativePreConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	void OnMouseButtonUp();	
};
