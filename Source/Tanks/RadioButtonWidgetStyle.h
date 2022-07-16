#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"
#include "RadioButtonWidgetStyle.generated.h"

USTRUCT()
struct TANKS_API FRadioButtonStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FRadioButtonStyle();
	virtual ~FRadioButtonStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FRadioButtonStyle& GetDefault();

	/** Style of check boxes */
	UPROPERTY(EditAnywhere, Category = Appearance)
	FCheckBoxStyle CheckBoxStyle;

	/** Style of options text */
	UPROPERTY(EditAnywhere, Category = Appearance)
	FTextBlockStyle TextStyle;
};

UCLASS(hidecategories=Object, MinimalAPI)
class URadioButtonWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FRadioButtonStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
