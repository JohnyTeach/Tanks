#include "MainMenuWidget.h"
#include "StyleSet.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(NewGameButton)
	{
		NewGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGameClicked);
	}

	if(OptionsButton)
	{
		OptionsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnOptionsClicked);
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBackClicked);
	}

	if(ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitClicked);
	}

	if(ChangeStyleButton)
	{
		ChangeStyleButton->OnClicked.AddDynamic(this, &UMainMenuWidget::SwapRadioButtonStyle);
	}
}

//================================================= УДАЛИТЬ ============================================================
void UMainMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if(RadioButton)
	{
		RadioButton->WidgetStyle = FStyleSet::Get().GetWidgetStyle<FRadioButtonStyle>( RadioButtonStyle );
	}

	if (ChangeStyleButton)
	{
		ChangeStyleButton->WidgetStyle = FStyleSet::Get().GetWidgetStyle<FButtonStyle>( ChangeStyleButtonStyle );
	}	
}

void UMainMenuWidget::SwapRadioButtonStyle() 
{	
	if (RadioButtonStyle == "STYLE_RadioButton")
	{
		RadioButtonStyle = "STYLE_SecondRadioButton";		
	}
	else
	{
		RadioButtonStyle = "STYLE_RadioButton";		
	}
	
	NativePreConstruct();	
}
//================================================= УДАЛИТЬ ============================================================

void UMainMenuWidget::OnNewGameClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), LoadLevelName);
}

void UMainMenuWidget::OnOptionsClicked()
{
	MainMenuWSwitcher->SetActiveWidgetIndex(1);
}

void UMainMenuWidget::OnBackClicked()
{
	MainMenuWSwitcher->SetActiveWidgetIndex(0);
}

void UMainMenuWidget::OnExitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}