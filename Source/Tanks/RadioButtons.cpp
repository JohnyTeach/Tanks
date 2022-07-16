#include "RadioButtons.h"

void URadioButtons::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyRadioButtons.Reset();
}

TSharedRef<SWidget> URadioButtons::RebuildWidget()
{	
	MyRadioButtons = SNew(SRadioButton)
	.Style(&WidgetStyle)
	.OnRadioChoiceChanged(BIND_UOBJECT_DELEGATE(FOnRadioChoiceChanged, HandleOnRadioChoiceChanged))
	.CountCheckBox(GetCountCheckBox())
	.RadioIndex(GetRadioIndex());
	
	return MyRadioButtons.ToSharedRef();
}

void URadioButtons::HandleOnRadioChoiceChanged(int32 NewRadioChoice)
{
	if (OnRadioChoiceChanged.IsBound())
	{
		OnRadioChoiceChanged.Broadcast(NewRadioChoice);
	}	
}

int32 URadioButtons::GetCountCheckBox() const
{
	return CountCheckBox;
}

int32 URadioButtons::GetRadioIndex() const
{
	return StartingIndex;
}

void URadioButtons::SetCountCheckBox(int32 NewCount)
{
	MyRadioButtons->SCountCheckBox = NewCount;
}

#if WITH_EDITOR
void URadioButtons::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);   

	StartingIndex = FMath::Clamp(StartingIndex, 0, CountCheckBox);
}
#endif