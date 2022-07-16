#pragma once

#include "CoreMinimal.h"
#include "RadioButtons.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuWidget.generated.h"

DECLARE_EVENT(UMainMenuWidget, ZALUPA);

UCLASS()
class TANKS_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	ZALUPA zal;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UWidgetSwitcher* MainMenuWSwitcher;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* NewGameButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* OptionsButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* ExitButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LoadLevelName;
	
	virtual void NativeConstruct() override;

	//================================================= УДАЛИТЬ ============================================================
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	URadioButtons* RadioButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* ChangeStyleButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RadioButtonStyle = "STYLE_RadioButton";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ChangeStyleButtonStyle = "STYLE_BackButton";

	UFUNCTION(BlueprintCallable)
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SwapRadioButtonStyle();
	//================================================= УДАЛИТЬ ============================================================
	
	UFUNCTION()
	void OnNewGameClicked();

	UFUNCTION()
	void OnOptionsClicked();

	UFUNCTION()
	void OnBackClicked();

	UFUNCTION()
	void OnExitClicked();
};