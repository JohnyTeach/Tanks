#include "RadioButtonWidgetStyle.h"

FRadioButtonStyle::FRadioButtonStyle()
{
	CheckBoxStyle = FCoreStyle::Get().GetWidgetStyle<FCheckBoxStyle>("Checkbox");
	
	TextStyle = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText");
}

FRadioButtonStyle::~FRadioButtonStyle()
{
}

const FName FRadioButtonStyle::TypeName(TEXT("FRadioButtonStyle"));

const FRadioButtonStyle& FRadioButtonStyle::GetDefault()
{
	static FRadioButtonStyle Default;
	return Default;
}

void FRadioButtonStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

