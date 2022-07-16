#include "RadioButton.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SRadioButton::Construct(const FArguments& InArgs)
{
	OnRadioChoiceChanged = InArgs._OnRadioChoiceChanged;

	RadioIndex = InArgs._RadioIndex;

	CurrentChoice = RadioIndex.Get();

	SCountCheckBox = InArgs._CountCheckBox;

	SetRadioButtonStyle(InArgs._Style);
	
	ChildSlot
	[
		SAssignNew(VerticalBox,SVerticalBox)		
	];		 
}

void SRadioButton::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	
	if (CurrentCountCheckBox != SCountCheckBox.Get())
	{
		VerticalBox->ClearChildren();
		
		for (int32 i = 1; i <= SCountCheckBox.Get(); ++i)
		{
			FString Num = FString::FromInt(i);
			
			VerticalBox->AddSlot()
			[
				CreateRadioButton("Options " + Num,  i)				
			];
		}

		CurrentCountCheckBox = SCountCheckBox.Get();
	}		
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SRadioButton::SetRadioButtonStyle(const FRadioButtonStyle* InStyle)
{
	CheckBoxStyle = &InStyle->CheckBoxStyle;
	
	TextStyle = &InStyle->TextStyle;
}

TSharedRef<SWidget> SRadioButton::CreateRadioButton(const FString& RadioText, int32 RadioButtonChoice)
{
	return SNew(SCheckBox)
			.IsChecked(MakeAttributeRaw(this, &SRadioButton::IsRadioButtonChecked, RadioButtonChoice))
			.OnCheckStateChanged(this, &SRadioButton::HandleRadioButtonStateChanged, RadioButtonChoice)
			.Style(CheckBoxStyle)
			[
				SNew(STextBlock)
				.Text(FText::FromString(RadioText))
				.TextStyle(TextStyle)
			];
}

ECheckBoxState SRadioButton::IsRadioButtonChecked(int32 RadioButtonID)
{
	return(CurrentChoice == RadioButtonID) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SRadioButton::HandleRadioButtonStateChanged(ECheckBoxState NewRadioState, int32 RadioButtonID)
{
	if(NewRadioState == ECheckBoxState::Checked)
	{
		CurrentChoice = RadioButtonID;
		OnRadioChoiceChanged.ExecuteIfBound(CurrentChoice); 
	}
}