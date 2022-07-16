#include "PlayerStatusWidget.h"
#include "UniversalPawn.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"


void UPlayerStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerStatusWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);	
	
	if (Cast<AUniversalPawn>(GetOwningPlayerPawn())->GetCannon())
	{
		float Ammo = Cast<AUniversalPawn>(GetOwningPlayerPawn())->GetCannon()->GetAmmo();
		
		float AmmoState = Cast<AUniversalPawn>(GetOwningPlayerPawn())->GetCannon()->GetAmmoState();
		
		AmmoBar->SetPercent(AmmoState);
		
		AmmoCount->SetText(UKismetTextLibrary::Conv_FloatToText(Ammo,ERoundingMode::HalfFromZero));			
	}

	if (Cast<AUniversalPawn>(GetOwningPlayerPawn()))
	{
		float HPState = Cast<AUniversalPawn>(GetOwningPlayerPawn())->GetHealthComponent()->GetHealthState();
		
		float HP = Cast<AUniversalPawn>(GetOwningPlayerPawn())->GetHealthComponent()->GetHealth();
		
		float Score = Cast<AUniversalPawn>(GetOwningPlayerPawn())->Scores;
		
		HPBar->SetPercent(HPState);
		
		HPCount->SetText(UKismetTextLibrary::Conv_FloatToText(HP,ERoundingMode::HalfFromZero));
		
		PlayerScore->SetText(UKismetTextLibrary::Conv_FloatToText(Score,ERoundingMode::HalfFromZero));		
	}		
}
