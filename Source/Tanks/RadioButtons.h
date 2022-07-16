#pragma once

#include "RadioButtonWidgetStyle.h"
#include "CoreMinimal.h"
#include "RadioButton.h"
#include "Components/Widget.h"
#include "RadioButtons.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadioChoiceChangedEvent, int32, NewRadioChoise);

UCLASS()
class TANKS_API URadioButtons : public UWidget
{
	GENERATED_BODY()
	
public:
	
	/** Called when radio choice changed by click */
	UPROPERTY(BlueprintAssignable, Category = "RadioButtons|Event")
	FOnRadioChoiceChangedEvent OnRadioChoiceChanged;

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

	TSharedPtr<SRadioButton> MyRadioButtons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	int32 CountCheckBox = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	int32 StartingIndex = 1;

	UPROPERTY(EditAnywhere, Category = "Appearance", meta = (DisplayName = "Style"))
	FRadioButtonStyle WidgetStyle;
	
	void HandleOnRadioChoiceChanged(int32 NewRadioChoice);

	int32 GetCountCheckBox() const;

	int32 GetRadioIndex() const;

	UFUNCTION(BlueprintCallable)
	void SetCountCheckBox(int32 NewCount);

	#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif
};