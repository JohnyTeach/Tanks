#pragma once

#include "RadioButtonWidgetStyle.h"
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FOnRadioChoiceChanged, int32 /*NewRadioChoise*/);

class TANKS_API SRadioButton : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRadioButton)
	{}

	SLATE_EVENT(FOnRadioChoiceChanged, OnRadioChoiceChanged)

	SLATE_ATTRIBUTE(int32, CountCheckBox)

	SLATE_ATTRIBUTE(int32, RadioIndex)

	SLATE_STYLE_ARGUMENT(FRadioButtonStyle, Style)
	
	SLATE_END_ARGS()

	int32 CurrentChoice;	

	FOnRadioChoiceChanged OnRadioChoiceChanged;	
	
	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	void SetRadioButtonStyle(const FRadioButtonStyle* InStyle);

	const FCheckBoxStyle* CheckBoxStyle;
	
	const FTextBlockStyle* TextStyle;
	
	ECheckBoxState IsRadioButtonChecked(int32 RadioButtonID);
	
	void HandleRadioButtonStateChanged(ECheckBoxState NewRadioState, int32 RadioButtonID);

	TSharedRef<SWidget> CreateRadioButton(const FString& RadioText, int32 RadioButtonChoice);
		
	TSharedPtr<SVerticalBox> VerticalBox;

	TAttribute<int32> SCountCheckBox;

	TAttribute<int32> RadioIndex;

	int32 CurrentCountCheckBox = 0;
};