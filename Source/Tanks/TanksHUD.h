#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TanksHUD.generated.h"

UENUM()
enum class EWidgetID: uint8
{
	None,
	MainMenu,
	PlayerStatus,
	GameOver,
	Inventory
};

UCLASS()
class TANKS_API ATanksHUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TMap<EWidgetID, TSubclassOf<UUserWidget>> WidgetClasses;

	UPROPERTY()
	UUserWidget* CurrentWidget;

	EWidgetID CurrentWidgetID;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	UUserWidget* UseWidget(EWidgetID WidgetID, bool RemovePrevious = true, int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetCurrentWidget() const;

	UFUNCTION(BlueprintCallable)
	void RemoveCurrentWidgetFromViewport();

	UFUNCTION(BlueprintCallable)
	UUserWidget* AddWidgetToViewportByClass( TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder = 0);
};
