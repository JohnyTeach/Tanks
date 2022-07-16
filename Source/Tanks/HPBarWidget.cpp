#include "HPBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void UHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHPBarWidget::SetHPValue(float HPValue, float HP) const
{	
	HPCount->SetText(UKismetTextLibrary::Conv_FloatToText(HP,ERoundingMode::HalfFromZero));
	
	HPBar->SetPercent(HPValue);		
}