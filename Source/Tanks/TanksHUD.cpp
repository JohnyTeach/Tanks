#include "TanksHUD.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void ATanksHUD::BeginPlay()
{
	Super::BeginPlay();

	if(GetWorld())
	{
		APlayerController * PC = GetWorld()->GetFirstPlayerController();
		
		if(PC)
		{
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx( PC,nullptr, EMouseLockMode::DoNotLock,false);

			PC->bShowMouseCursor = true;
		}
	}
	
	if (GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		UseWidget(EWidgetID::PlayerStatus);
	}
	else
	{
		UseWidget(EWidgetID::MainMenu);
	}
}

UUserWidget* ATanksHUD::GetCurrentWidget() const
{
	return CurrentWidget;
}

void ATanksHUD::RemoveCurrentWidgetFromViewport()
{
	if(CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();

		CurrentWidget = nullptr;

		CurrentWidgetID = EWidgetID::None;
	}
}

UUserWidget* ATanksHUD::AddWidgetToViewportByClass( TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder)
{
	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

	if(CurrentWidget)
	{
		CurrentWidget->AddToViewport(ZOrder);
	}
	
	return CurrentWidget;
}

UUserWidget* ATanksHUD::UseWidget(EWidgetID WidgetID,bool RemovePrevious, int32 ZOrder)
{
	if(CurrentWidgetID == WidgetID)
	{
		return CurrentWidget;
	}
	
	if(RemovePrevious)
	{
		RemoveCurrentWidgetFromViewport();
	}

	const TSubclassOf<UUserWidget> WidgetClassToUse = WidgetClasses.FindRef(WidgetID);

	if(WidgetClassToUse.Get())
	{
		CurrentWidgetID = WidgetID;

		return AddWidgetToViewportByClass(WidgetClassToUse,ZOrder);
	}

	return nullptr;
}