#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatusWidget.generated.h"

UCLASS()
class TANKS_API UPlayerStatusWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UProgressBar* HPBar;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UTextBlock* HPCount;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UProgressBar* AmmoBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UTextBlock* AmmoCount;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UTextBlock* PlayerScore;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	class UOverlay* TurretOverlay;

	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;	
};
